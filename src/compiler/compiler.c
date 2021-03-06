#include "compiler.h"

bool compiler_init(compiler* comp) {
	setlocale(LC_ALL, "en_US.utf8");

	vector_init(cctl_ptr(char), &comp->textcode_vector);
	vector_init(cctl_ptr(char), &comp->filename_vector);
	vector_init(cctl_ptr(char), &comp->preproc_tokens_vector);
	vector_init(size_t, &comp->textcode_index_stack);
	vector_init(uint8_t, &comp->bytecode);
	vector_init(cctl_ptr(vector(control_data)), &comp->control_data_stack);
	trie_init(&comp->dictionary);
	trie_init(&comp->filename_trie);

	comp->dictionary_keyword_count = 0;
	comp->line_count = 1;
	comp->column_count = 0;

	trie* word;

	for (int i = 0; i < control_len; i++) {
		word = trie_insert(&comp->dictionary, control_names[i], WTT_CTRL);
		if (!word) goto FAILURE_DICT;
		word->data.u = i;
	}

	for (int i = 0; i < operation_len; i++) {
		word = trie_insert(&comp->dictionary, operation_names[i], WTT_OP);
		if (!word) goto FAILURE_DICT;
		word->data.u = operation_indices[i];
	}

	return true;

FAILURE_DICT:
	fputs("error : Dictionary memory allocation failure\n", stderr);
	return false;
}

bool compiler_del(compiler* comp) {
	for (size_t i = 0; i < comp->textcode_vector.size; i++) {
		free(*vector_at(cctl_ptr(char), &comp->textcode_vector, i));
		free(*vector_at(cctl_ptr(char), &comp->filename_vector, i));
	}

	for (size_t i = 0; i < comp->preproc_tokens_vector.size; i++) {
		free(*vector_at(cctl_ptr(char), &comp->preproc_tokens_vector, i));
	}

	vector_free(cctl_ptr(char), &comp->textcode_vector);
	vector_free(cctl_ptr(char), &comp->filename_vector);

	vector_free(size_t, &comp->textcode_index_stack);
	vector_free(uint8_t, &comp->bytecode);
	for (size_t i = 0; i < comp->control_data_stack.size; i++) {
		vector_free(control_data, *vector_at(cctl_ptr(vector(control_data)), &comp->control_data_stack, i));
	}
	vector_free(cctl_ptr(vector(control_data)), &comp->control_data_stack);
	trie_del(&comp->dictionary);
	trie_del(&comp->filename_trie);

	return true;
}

bool compiler_compile(compiler* comp, char* input_filename, char* output_filename) {
	if (!compiler_compile_source(comp, input_filename)) return false;
	if (!compiler_save_code(comp, output_filename)) {
		fputs("error : File saving failure\n", stderr);
		return false;
	}
	return true;
}

bool compiler_compile_source(compiler* comp, char* input_filename) {
	int index = compiler_load_code(comp, input_filename);
	if (!index) {
		fputs("error : Loading code failure\n", stderr);
		return false;
	}
	if (!vector_push_back(size_t, &comp->textcode_index_stack, index - 1)) {
		fputs("error : Textcode index stack memory allocation failure\n", stderr);
		return false;
	}
	if (!compiler_tokenize(comp)) {
		fputs("error : Tokenization failure\n", stderr);
		return false;
	}
	if (!vector_pop_back(size_t, &comp->textcode_index_stack)) {
		fputs("error : Textcode index stack memory allocation failure\n", stderr);
		return false;
	}
	return true;
}

size_t compiler_load_code(compiler* comp, char* filename) {
	FILE* file;
	size_t size;

	char filename_full[PATH_MAX];

#ifdef _WIN32
	if (!(_fullpath(filename_full, filename, PATH_MAX))) goto FAILURE_FILEPATH;
#else
	if (!(realpath(filename, filename_full))) goto FAILURE_FILEPATH;
#endif

	file = fopen(filename_full, "rb");
	if (!file) {
		fprintf(stderr, console_yellow console_bold "%s" console_reset "\n", filename_full);
		fputs("error : File reading failure\n", stderr);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	int filename_size = strlen(filename_full) + 1;

	char* textcode = (char*) malloc(size + 2);
	char* filename_full_new = (char*) malloc(filename_size);
	if (!(textcode && filename_full_new)) {
		fclose(file);
		fputs("error : Textcode memory allocation failure\n", stderr);
		return 0;
	}

	int read_result = fread(textcode, size, 1, file);
	if (read_result != 1) {
		free(textcode);
		fclose(file);
		fputs("error : Entire reading failure\n", stderr);
		return 0;
	}

	textcode[size] = '\n';
	textcode[size + 1] = '\0';

#ifdef _WIN32
	memcpy_s(filename_full_new, filename_size, filename_full, filename_size);
#else
	memcpy(filename_full_new, filename_full, filename_size);
#endif

	trie* filename_trie_result;
	bool already_imported = false;
	filename_trie_result = trie_find(&comp->filename_trie, filename_full_new);
	if (filename_trie_result) if (filename_trie_result->type == (uint8_t) true) already_imported = true;
	if (!already_imported) filename_trie_result = trie_insert(&comp->filename_trie, filename_full_new, (uint8_t) true);

	if (!(
			vector_push_back(cctl_ptr(char), &comp->textcode_vector, textcode) &&
			vector_push_back(cctl_ptr(char), &comp->filename_vector, filename_full_new) &&
			filename_trie_result
		)) {
		free(textcode);
		free(filename_full_new);
		fclose(file);
		fputs("error : Textcode vector memory allocation failure\n", stderr);
		return 0;
	}

	filename_trie_result->data.u = comp->textcode_vector.size - 1;

	fclose(file);

	return comp->textcode_vector.size;

FAILURE_FILEPATH:
	fprintf(stderr, console_yellow console_bold "%s" console_reset "\n", filename_full);
	fprintf(stderr, "error : %s\n", strerror(errno));
	return 0;
}

bool compiler_save_code(compiler* comp, char* filename) {
	FILE* file;
	file = fopen(filename, "wb");

	size_t result = fwrite(comp->bytecode.p_data, 1, comp->bytecode.size, file);

	fclose(file);

	if (result != comp->bytecode.size) {
		fputs("error : File writing failure\n", stderr);
		return false;
	}

	return true;
}

bool compiler_tokenize(compiler* comp) {
	size_t index;

	if (comp->textcode_index_stack.size == 0) {
		fputs("error : Textcode index error\n", stderr);
		return false;
	}

	index = *vector_back(size_t, &comp->textcode_index_stack);

	char* iterator = *vector_at(cctl_ptr(char), &comp->textcode_vector, index);
	char* begin = NULL;
	char* end = NULL;

	bool string_escape = false;
	bool space = true;
	string_parse_mode string_parse = STR_PARSE_NONE;
	comment_parse_mode comment = CMNT_PARSE_NONE;
	bool result;

	bool u8check = false;

	while (*iterator) {
		switch (*iterator) {
			case '\n': 
				comp->line_count++;
			case '\r':
				comp->column_count = 0;
				if (comment == CMNT_PARSE_LINE) {
					space = true;
					comment = CMNT_PARSE_NONE;
				}
			case '\t':
			case ' ': {
				if (!comment) {
					if (!space) {
						if (string_parse) {
							if (string_escape) string_escape = false;
						}
						else {
							end = iterator;
							result = compiler_parse(comp, begin, end);
							if (!result) {
								fprintf(stderr, "from file " console_yellow console_bold "%s" console_reset "\n", *vector_at(cctl_ptr(char), &comp->filename_vector, index));
								fputs("error : Parse failure\n", stderr);
								return false;
							}
							space = true;
						}
					}
				}
			} break;
			case '\'': {
				if (!comment) {
					if (string_parse) {
						if (string_escape) string_escape = false;
						else {
							if (string_parse == STR_PARSE_SINGLE) {
								string_parse = STR_PARSE_NONE;
							}
						}
					}
					else if (space) {
						space = false;
						begin = iterator;
						string_parse = STR_PARSE_SINGLE;
						string_escape = false;
					}
				}
			} break;
			case '\"': {
				if (!comment) {
					if (string_parse) {
						if (string_escape) string_escape = false;
						else {
							if (string_parse == STR_PARSE_DOUBLE) {
								string_parse = STR_PARSE_NONE;
							}
						}
					}
					else if (space) {
						space = false;
						begin = iterator;
						string_parse = STR_PARSE_DOUBLE;
						string_escape = false;
					}
				}
			} break;
			case '\\': {
				if (!comment) {
					if (string_parse) {
						if (string_escape) string_parse = false;
						else string_parse = true;
					}
					else if (space) {
						space = false;
						comment = CMNT_PARSE_LINE;
					}
				}
			} break;
			case '(': {
				if (!comment) {
					if (string_parse) {
						if (string_escape) string_escape = false;
					}
					else if (space) {
						space = false;
						comment = CMNT_PARSE_STACK;
					}
				}
			} break;
			case ')': {
				if (!comment) {
					if (string_parse) {
						if (string_escape) string_escape = false;
					}
				}
				if (comment == CMNT_PARSE_STACK) {
					space = true;
					comment = CMNT_PARSE_NONE;
				}
			} break;
			default: {
				if (!comment) {
					if (string_parse) {
						if (string_escape) {
							string_escape = false;
						}
					}
					if (space) {
						space = false;
						begin = iterator;
					}
				}
			}
		}
		iterator++;
		
		if (((signed char) *iterator) >= -64) {
			comp->column_count++;
		}
		
	}

	if (comp->control_data_stack.size > 0) {
		fputs("error : Control level does not match\n", stderr);
		return false;
	}

	return true;
}

bool compiler_parse(compiler* comp, char* begin, char* end) {
	bool result;
	char temp = *end;
	*end = 0;

	trie* trie_result = trie_find(&comp->dictionary, begin);

	if (trie_result) {
		result = compiler_parse_word_token(comp, trie_result);
	}
	else {
		switch (*begin) {
			case '+': {
				result = compiler_parse_zero_begin_num(comp, begin, 1, false);
			} break;
			case '-': {
				result = compiler_parse_zero_begin_num(comp, begin, 1, true);
			} break;
			case '0': {
				result = compiler_parse_zero_begin_num(comp, begin, 0, false);
			} break;
			case '.':
			case '1' ... '9': {
				result = compiler_parse_num(comp, begin);
			} break;
			case '$': {
				result = compiler_parse_keyword_value(comp, begin + 1);
			} break;
			case '#': {
				result = compiler_push_preproc_token(comp, begin + 1);
			} break;
			case '\'': {
				char temp_parse_char = *(end - 1);
				*(end - 1) = 0;
				result = compiler_parse_char(comp, begin + 1, false);
				*(end - 1) = temp_parse_char;
			} break;
			case '\"': {
				char temp_parse_char = *(end - 1);
				*(end - 1) = 0;
				result = compiler_parse_char(comp, begin + 1, true);
				*(end - 1) = temp_parse_char;
			} break;
			default: {
				result = false;
				fputs("error : Unknown keyword\n", stderr);
			}
		}
	}

	if (!result) {
		fprintf(stderr, console_yellow console_bold "%s" console_reset " in line %zu, column %zu\n", begin, comp->line_count, comp->column_count);
	}

	*end = temp;
	return result;
}

bool compiler_parse_word_token(compiler* comp, trie* trie_result) {
	bool result = false;

	switch (trie_result->type) {
		case WTT_CTRL: {
			result = compiler_parse_control_words(comp, trie_result);
		} break;
		case WTT_KWRD: {
			value v = trie_result->data;
			if (!compiler_push_bytecode_with_value(comp, OP_CALL, v)) return false;
			result = true;
		} break;
		case WTT_OP: {
			value v = trie_result->data;
			if (!compiler_push_bytecode(comp, (opcode) (v.u))) return false;
			result = true;
		} break;
	}

	return result;
}

bool compiler_parse_control_words(compiler* comp, trie* trie_result) {
	control_data current_ctrl;
	current_ctrl.ctrl = trie_result->data.u;
	current_ctrl.pos = comp->bytecode.size;

	vector(control_data)* temp_ctrl_vec;

	switch (current_ctrl.ctrl) {
		case CTRL_IF: {
			temp_ctrl_vec = (vector(control_data)*) malloc(sizeof(vector(control_data)));
			if (!temp_ctrl_vec) goto FAILURE_CTRL_VECTOR;
			vector_init(control_data, temp_ctrl_vec);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!vector_push_back(cctl_ptr(vector(control_data)), &comp->control_data_stack, temp_ctrl_vec)) goto FAILURE_CTRL_STACK;
			if (!compiler_push_bytecode_with_null(comp, OP_IF)) return false;
		} break;
		case CTRL_ELSE: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode_with_null(comp, OP_JUMP)) return false;
		} break;
		case CTRL_LOOP: {
			temp_ctrl_vec = (vector(control_data)*) malloc(sizeof(vector(control_data)));
			if (!temp_ctrl_vec) goto FAILURE_CTRL_VECTOR;
			vector_init(control_data, temp_ctrl_vec);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!vector_push_back(cctl_ptr(vector(control_data)), &comp->control_data_stack, temp_ctrl_vec)) goto FAILURE_CTRL_STACK;
		} break;
		case CTRL_WHILE: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode_with_null(comp, OP_IF)) return false;
		} break;
		case CTRL_CONTINUE:
		case CTRL_BREAK: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode_with_null(comp, OP_JUMP)) return false;
		} break;
		case CTRL_SWITCH: {
			temp_ctrl_vec = (vector(control_data)*) malloc(sizeof(vector(control_data)));
			if (!temp_ctrl_vec) goto FAILURE_CTRL_VECTOR;
			vector_init(control_data, temp_ctrl_vec);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!vector_push_back(cctl_ptr(vector(control_data)), &comp->control_data_stack, temp_ctrl_vec)) goto FAILURE_CTRL_STACK;
			if (!compiler_push_bytecode(comp, OP_SWITCH)) return false;
		} break;
		case CTRL_CASE: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode(comp, OP_CASE)) return false;
			if (!compiler_push_bytecode(comp, OP_EQU)) return false;
			if (!compiler_push_bytecode_with_null(comp, OP_IF)) return false;
		} break;
		case CTRL_PASS: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode_with_null(comp, OP_JUMP)) return false;
		} break;
		case CTRL_FUNC: {
			temp_ctrl_vec = (vector(control_data)*) malloc(sizeof(vector(control_data)));
			if (!temp_ctrl_vec) goto FAILURE_CTRL_VECTOR;
			vector_init(control_data, temp_ctrl_vec);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!vector_push_back(cctl_ptr(vector(control_data)), &comp->control_data_stack, temp_ctrl_vec)) goto FAILURE_CTRL_STACK;
			if (!compiler_push_bytecode_with_null(comp, OP_FUNC)) return false;
		} break;
		case CTRL_MACRO: {
			temp_ctrl_vec = (vector(control_data)*) malloc(sizeof(vector(control_data)));
			if (!temp_ctrl_vec) goto FAILURE_CTRL_VECTOR;
			vector_init(control_data, temp_ctrl_vec);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!vector_push_back(cctl_ptr(vector(control_data)), &comp->control_data_stack, temp_ctrl_vec)) goto FAILURE_CTRL_STACK;
			if (!compiler_push_bytecode_with_null(comp, OP_MACRO)) return false;
		} break;
		case CTRL_RETURN: {
			if (!comp->control_data_stack.size) goto FAILURE_CTRL;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			if (!vector_push_back(control_data, temp_ctrl_vec, current_ctrl)) goto FAILURE_CTRL_VECTOR;
			if (!compiler_push_bytecode(comp, OP_RETURN)) return false;
		} break;
		case CTRL_END: {
			value pos;
			if (comp->control_data_stack.size == 0) goto FAILURE_CTRL_STACK;
			temp_ctrl_vec = *vector_back(cctl_ptr(vector(control_data)), &comp->control_data_stack);
			control_data* first_ctrl = vector_front(control_data, temp_ctrl_vec);
			control_data* while_else_ctrl = NULL;
			switch (first_ctrl->ctrl) {
				case CTRL_IF: {
					for (
						control_data* iter = vector_at(control_data, temp_ctrl_vec, 1);
						iter <= vector_back(control_data, temp_ctrl_vec);
						iter++
					) {
						switch (iter->ctrl) {
							case CTRL_ELSE: {
								while_else_ctrl = iter;
							} break;
							case CTRL_BREAK:
							case CTRL_CONTINUE:
							case CTRL_RETURN: {
								vector(control_data)* next_ctrl_vec;
								if (comp->control_data_stack.size < 2) goto FAILURE_CTRL_STACK;
								next_ctrl_vec = *vector_at(cctl_ptr(vector(control_data)), &comp->control_data_stack, comp->control_data_stack.size - 2);
								if (!vector_push_back(control_data, next_ctrl_vec, *iter)) goto FAILURE_CTRL_VECTOR;
							} break;
							default: {
								goto FAILURE_CTRL;
							}
						}
					}
					if (while_else_ctrl) {
						pos.u = while_else_ctrl->pos + 9;
						for (int i = 0; i < 8; i++) {
							*vector_at(uint8_t, &comp->bytecode, first_ctrl->pos + 1 + i) = pos.bytes[i];
						}
						pos.u = current_ctrl.pos;
						for (int i = 0; i < 8; i++) {
							*vector_at(uint8_t, &comp->bytecode, while_else_ctrl->pos + 1 + i) = pos.bytes[i];
						}
					}
					else {
						pos.u = current_ctrl.pos;
						for (int i = 0; i < 8; i++) {
							*vector_at(uint8_t, &comp->bytecode, first_ctrl->pos + 1 + i) = pos.bytes[i];
						}
					}
				} break;
				case CTRL_LOOP: {
					for (
						control_data* iter = vector_at(control_data, temp_ctrl_vec, 1);
						iter <= vector_back(control_data, temp_ctrl_vec);
						iter++
					) {
						switch (iter->ctrl) {
							case CTRL_WHILE: {
								while_else_ctrl = iter;
								pos.u = current_ctrl.pos + 9;
								for (int i = 0; i < 8; i++) {
									*vector_at(uint8_t, &comp->bytecode, while_else_ctrl->pos + 1 + i) = pos.bytes[i];
								}
							} break;
							case CTRL_BREAK: {
								pos.u = current_ctrl.pos + 9;
								for (int i = 0; i < 8; i++) {
									*vector_at(uint8_t, &comp->bytecode, iter->pos + 1 + i) = pos.bytes[i];
								}
							} break;
							case CTRL_CONTINUE: {
								pos.u = first_ctrl->pos;
								for (int i = 0; i < 8; i++) {
									*vector_at(uint8_t, &comp->bytecode, iter->pos + 1 + i) = pos.bytes[i];
								}
							} break;
							case CTRL_RETURN: {
								vector(control_data)* next_ctrl_vec;
								if (comp->control_data_stack.size < 2) goto FAILURE_CTRL_STACK;
								next_ctrl_vec = *vector_at(cctl_ptr(vector(control_data)), &comp->control_data_stack, comp->control_data_stack.size - 2);
								if (!vector_push_back(control_data, next_ctrl_vec, *iter)) goto FAILURE_CTRL_VECTOR;
							} break;
							default: {
								goto FAILURE_CTRL;
							}
						}
					}
					pos.u = first_ctrl->pos;
					if (!compiler_push_bytecode_with_value(comp, OP_JUMP, pos)) return false;
				} break;
				case CTRL_SWITCH: {
					#define __free_switch_vecs__ \
						vector_free(control_data, &case_vec); \
						vector_free(control_data, &pass_vec); \
						vector_free(control_data, &chain_vec);
					
					vector(control_data) case_vec;
					vector(control_data) pass_vec;
					vector(control_data) chain_vec;

					vector_init(control_data, &case_vec);
					vector_init(control_data, &pass_vec);
					vector_init(control_data, &chain_vec);

					pos.u = current_ctrl.pos;

					bool chain = false;

					bool case_existance = false;
					bool case_first = false;
					bool pass_existance = false;

					for (
						control_data* iter = vector_at(control_data, temp_ctrl_vec, 1);
						iter <= vector_back(control_data, temp_ctrl_vec);
						iter++
					) {
						switch (iter->ctrl) {
							case CTRL_CASE: {
								if (chain) {
									if (!vector_push_back(control_data, &pass_vec, *iter)) {
										__free_switch_vecs__;
										goto FAILURE_CTRL_VECTOR;
									}
								}
								if (!vector_push_back(control_data, &case_vec, *iter)) {
									__free_switch_vecs__;
									goto FAILURE_CTRL_VECTOR;
								}
								chain = true;
								case_existance = true;
							} break;
							case CTRL_PASS: {
								chain = false;
								for (int i = 0; i < 8; i++)
									*vector_at(uint8_t, &comp->bytecode, iter->pos + 1 + i) = pos.bytes[i];
								if (!vector_push_back(control_data, &pass_vec, *iter)) {
									__free_switch_vecs__;
									goto FAILURE_CTRL_VECTOR;
								}
								pass_existance = true;
							} break;
							case CTRL_BREAK:
							case CTRL_CONTINUE:
							case CTRL_RETURN: {
								vector(control_data)* next_ctrl_vec;
								if (comp->control_data_stack.size < 2) {
									__free_switch_vecs__;
									goto FAILURE_CTRL_STACK;
								}
								next_ctrl_vec = *vector_at(cctl_ptr(vector(control_data)), &comp->control_data_stack, comp->control_data_stack.size - 2);
								if (!vector_push_back(control_data, next_ctrl_vec, *iter)) {
									__free_switch_vecs__;
									goto FAILURE_CTRL_VECTOR;
								}
							} break;
							default: {
								__free_switch_vecs__;
								goto FAILURE_CTRL;
							}
						}
					}

					if (!(
						pass_existance && case_existance &&
						(pass_existance ? (vector_at(control_data, temp_ctrl_vec, 1)->ctrl == CTRL_CASE) : 0)
					)) {
						__free_switch_vecs__;
						goto FAILURE_CTRL;
					}

					control_data* iter_case = vector_front(control_data, &case_vec);
					control_data* iter_pass = vector_front(control_data, &pass_vec);

					for (; iter_case <= vector_back(control_data, &case_vec); iter_case++) {
						if (iter_pass->ctrl == CTRL_PASS) {
							if (chain_vec.size) {
								for (
									control_data* iter_chain = vector_front(control_data, &chain_vec);
									iter_chain <= vector_back(control_data, &chain_vec);
									iter_chain++
								) {
									pos.u = iter_case->pos + 11;
									for (int i = 0; i < 8; i++) {
										*vector_at(uint8_t, &comp->bytecode, iter_chain->pos + 3 + i) = pos.bytes[i];
									}
									*vector_at(uint8_t, &comp->bytecode, iter_chain->pos + 1) = OP_NEQ;
								}
								vector_clear(control_data, &chain_vec);
							}

							pos.u = iter_pass->pos + 9;
							for (int i = 0; i < 8; i++) {
								*vector_at(uint8_t, &comp->bytecode, iter_case->pos + 3 + i) = pos.bytes[i];
							}
							iter_pass++;
						}
						else {
							if (!vector_push_back(control_data, &chain_vec, *iter_case)) {
								__free_switch_vecs__;
								goto FAILURE_CTRL_VECTOR;
							}
							iter_pass++;
						}
					}

					__free_switch_vecs__;
					
					if (!compiler_push_bytecode(comp, OP_ENDSWITCH)) return false;

					#undef __free_switch_vecs__
				} break;
				case CTRL_FUNC: {
					for (
						control_data* iter = vector_at(control_data, temp_ctrl_vec, 1);
						iter <= vector_back(control_data, temp_ctrl_vec);
						iter++
					) {
						switch (iter->ctrl) {
							case CTRL_RETURN: {
							} break;
							default: {
								goto FAILURE_CTRL;
							}
						}
					}
					pos.u = current_ctrl.pos + 1;
					for (int i = 0; i < 8; i++) {
						*vector_at(uint8_t, &comp->bytecode, first_ctrl->pos + 1 + i) = pos.bytes[i];
					}
					if (!compiler_push_bytecode(comp, OP_RETURN)) return false;
				} break;
				case CTRL_MACRO: {
					for (
						control_data* iter = vector_at(control_data, temp_ctrl_vec, 1);
						iter <= vector_back(control_data, temp_ctrl_vec);
						iter++
					) {
						switch (iter->ctrl) {
							case CTRL_RETURN: {
								*vector_at(uint8_t, &comp->bytecode, iter->pos) = OP_ENDMACRO;
							} break;
							default: {
								goto FAILURE_CTRL;
							}
						}
					}
					pos.u = current_ctrl.pos + 1;
					for (int i = 0; i < 8; i++) {
						*vector_at(uint8_t, &comp->bytecode, first_ctrl->pos + 1 + i) = pos.bytes[i];
					}
					if (!compiler_push_bytecode(comp, OP_ENDMACRO)) return false;

				} break;
			}
			vector_free(control_data, temp_ctrl_vec);
			if (!vector_pop_back(cctl_ptr(vector(control_data)), &comp->control_data_stack)) goto FAILURE_CTRL_STACK;
		} break;
		case CTRL_IMPORT: {
			char* current_filename;

			char* token;
			size_t index;

			if (comp->textcode_index_stack.size == 0) goto FAILURE_TEXTCODE;
			index = *vector_back(size_t, &comp->textcode_index_stack);

			if (comp->filename_vector.size == 0) goto FAILURE_TEXTCODE;
			current_filename = *vector_at(cctl_ptr(char), &comp->filename_vector, index);

			if (comp->preproc_tokens_vector.size == 0) goto FAILURE_PREPROC_STACK;
			token = *vector_back(cctl_ptr(char), &comp->preproc_tokens_vector);

			char import_filename[PATH_MAX];

		#ifdef _WIN32
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];

			_splitpath(current_filename, drive, dir, NULL, NULL);
			_makepath(import_filename, drive, dir, token, NULL);
		#else
			char* dir = (char*) calloc(PATH_MAX, sizeof(char));
			memcpy(dir, current_filename, strlen(current_filename) + 1);
			dir = dirname(dir);
			memcpy(import_filename, dir, strlen(dir) + 1);
			free(dir);
			strcat(import_filename, "/");
			strcat(import_filename, token);
		#endif
			trie* filename_trie_result = trie_find(&comp->filename_trie, import_filename);

			if (filename_trie_result) {
				if (filename_trie_result->type == (uint8_t) true) {
					return true;
				}
			}

			if (!compiler_compile_source(comp, import_filename)) return false;
		} break;
	}

	return true;
FAILURE_CTRL:
	fputs("error : Invalid control syntax\n", stderr);
	return false;
FAILURE_CTRL_VECTOR:
	fputs("error : Control data vector memory allocation faliure\n", stderr);
	return false;
FAILURE_CTRL_STACK:
	fputs("error : Control data stack memory allocation faliure\n", stderr);
	return false;
FAILURE_PREPROC_STACK:
	fputs("error : Preprocessor tokens stack memory allocation faliure\n", stderr);
	return false;
FAILURE_TEXTCODE:
	fputs("error : Filename vector or textcode indices memory stack memory allocation faliure\n", stderr);
	return false;
}

bool compiler_parse_keyword_value(compiler* comp, char* token) {
	trie* word;
	
	word = trie_find(&comp->dictionary, token);

	if (word) {
		if (word->type != WTT_KWRD) {
			fputs("error : Compile-time keywords, control words and built-in-functions cannot be keyword\n", stderr);
			return false;
		}
	}
	else {
		char* iter = token;
		switch (*iter) {
			case '+':
			case '-':
			case '.': {
				iter++;
				switch (*iter) {
					case '0' ... '9': {
						goto FAILURE_INVALID_KEYWORD;
					} break;
				}
			} break;
			case '0' ... '9':
			case '@':
			case '#':
			case '$':
			case '\'':
			case '\"':
			case '\0': {
				goto FAILURE_INVALID_KEYWORD;
			} break;
		}

		comp->dictionary_keyword_count++;
		word = trie_insert(&comp->dictionary, token, WTT_KWRD);
		if (!word) {
			fputs("error : Dictionary memory allocation failure\n", stderr);
			return false;
		}
		word->data.u = comp->dictionary_keyword_count;
	}

	if (!compiler_push_bytecode_with_value(comp, OP_VALUE, word->data)) return false;

	return true;

FAILURE_INVALID_KEYWORD:
	fputs("error : Invalid keyword\n", stderr);
	return false;
}

bool compiler_parse_zero_begin_num(compiler* comp, char* token, size_t index, bool negate) {
	bool result;

	if (token[index] == '0') {
		switch (token[index + 1]) {
			case 'x': {
				result = compiler_parse_base_n_num(comp, token, index, negate, 16);
			} break;
			case 'o': {
				result = compiler_parse_base_n_num(comp, token, index, negate, 8);
			} break;
			case 'b': {
				result = compiler_parse_base_n_num(comp, token, index, negate, 2);
			} break;
			default: {
				result = compiler_parse_num(comp, token);
			}
		}
	}
	else result = compiler_parse_num(comp, token);

	return result;
}

bool compiler_parse_base_n_num(compiler* comp, char* token, size_t index, bool negate, int base) {
	char* temp = token + index + 2;
	char* stop;
	bool result;
	value v;

	errno = 0;
	v.i = strtoll(temp, &stop, base);

	if (errno == ERANGE) {
		if (v.i == LLONG_MAX) {
			v.u = strtoull(temp, &stop, base);
		}
	}
	if (negate) v.i = -v.i;
	
	if (*stop) {
		fputs("error : Number parsing failure\n", stderr);
		return false;
	}

	result = compiler_push_bytecode_with_value(comp, OP_VALUE, v);

	return result;
}

bool compiler_parse_num(compiler* comp, char* token) {
	char* stop;
	bool result;
	value v;

	errno = 0;
	if (strchr(token, '.')) {
		v.f = strtod(token, &stop);
	}
	else {
		v.i = strtoll(token, &stop, 10);
		if (errno == ERANGE) {
			if (v.i == LLONG_MAX) {
				v.u = strtoull(token, &stop, 10);
			}
		}
	}
	if (*stop) {
		fputs("error : Number parsing failure\n", stderr);
		return false;
	}
	result = compiler_push_bytecode_with_value(comp, OP_VALUE, v);

	return result;
}

bool compiler_parse_char(compiler* comp, char* token, bool push_length) {
	value v;

	int num_parse_count;
	int num_parse_max;

	vector(value) value_reverser;
	vector_init(value, &value_reverser);

	char* end = token + strlen(token) + 1;

	while (*token) {
		if (((signed char)*token) > -1) {
			char* num_pasre_stop = NULL;
			char num_parse[9] = {0, };
			num_parse_count = 0;
			if (*token == '\\') {
				token++;
				switch (*token) {
					case 'a': v.u = '\a'; break;
					case 'b': v.u = '\b'; break;
					case 'e': v.u = '\e'; break;
					case 'f': v.u = '\f'; break;
					case 'n': v.u = '\n'; break;
					case 'r': v.u = '\r'; break;
					case 't': v.u = '\t'; break;
					case 'v': v.u = '\v'; break;
					case '\\': v.u = '\\'; break;
					case '\'': v.u = '\''; break;
					case '\"': v.u = '\"'; break;
					case '0' ... '7': {
						while ((*token >= '0') && (*token <= '7') && (num_parse_count < 3)) {
							num_parse[num_parse_count] = *token;
							token++;
							num_parse_count++;
						}
						token--;
						v.u = strtoull(num_parse, &num_pasre_stop, 8);
					} break;
					case 'x': {
						token++;
						while (
							(((*token >= '0') && (*token <= '9')) || ((*token >= 'a') && (*token <= 'f')) || ((*token >= 'A') && (*token <= 'F'))) && (num_parse_count < 2)
						) {
							num_parse[num_parse_count] = *token;
							token++;
							num_parse_count++;
						}
						if (num_parse_count != 2) {
							goto FAILURE_ESCAPE;
						}
						token--;
						v.u = strtoull(num_parse, &num_pasre_stop, 16);
					} break;
					case 'u': {
						token++;
						while (
							(((*token >= '0') && (*token <= '9')) || ((*token >= 'a') && (*token <= 'f')) || ((*token >= 'A') && (*token <= 'F'))) && (num_parse_count < 4)
						) {
							num_parse[num_parse_count] = *token;
							token++;
							num_parse_count++;
						}
						if (num_parse_count != 4) {
							goto FAILURE_ESCAPE;
						}
						token--;
						v.u = strtoull(num_parse, &num_pasre_stop, 16);
					} break;
					case 'U': {
						token++;
						while (
							(((*token >= '0') && (*token <= '9')) || ((*token >= 'a') && (*token <= 'f')) || ((*token >= 'A') && (*token <= 'F'))) && (num_parse_count < 8)
						) {
							num_parse[num_parse_count] = *token;
							token++;
							num_parse_count++;
						}
						if (num_parse_count != 8) {
							goto FAILURE_ESCAPE;
						}
						token--;
						v.u = strtoull(num_parse, &num_pasre_stop, 16);
					} break;
					default: {
						goto FAILURE_ESCAPE;
					}
				}
			}
			else if ((*token == '\'') || (*token == '\"')) {
				vector_free(value, &value_reverser);
				fputs("error : String parse failure\n", stderr);
				return false;
			}
			else {
				v.u = *token;
			}
			token++;
		}
		else {
			char32_t out;
			size_t rc;
			mbstate_t state;
			rc = mbrtoc32(&out, token, end - token, &(comp->convert_state));
			if ((rc > ((size_t) -4)) || (rc == 0)) {
				goto FAILURE_UNICODE;
			}
			token += rc;
			v.u = out;
		}
		if (!vector_push_back(value, &value_reverser, v)) goto FAILURE_VECTOR;
	}
	
	for (size_t i = value_reverser.size - 1; i < -1; i--) {
		if (!compiler_push_bytecode_with_value(comp, OP_VALUE, *vector_at(value, &value_reverser, i))) return false;
	}

	if (push_length) {
		v.u = value_reverser.size;
		if (!compiler_push_bytecode_with_value(comp, OP_VALUE, v)) return false;
	}

	vector_free(value, &value_reverser);
	return true;

FAILURE_ESCAPE:
	vector_free(value, &value_reverser);
	fputs("error : Escape sequence parsing faliure\n", stderr);
	return false;

FAILURE_UNICODE:
	vector_free(value, &value_reverser);
	fputs("error : Unicode decoding faliure\n", stderr);
	return false;

FAILURE_VECTOR:
	vector_free(value, &value_reverser);
	fputs("error : Unicode inserter vector memory allocation failure\n", stderr);
	return false;
}

bool compiler_push_bytecode(compiler* comp, opcode op) {
	if (!vector_push_back(uint8_t, &comp->bytecode, (uint8_t) op)) {
		fputs("error : Bytecode memory allocation faliure\n", stderr);
		return false;
	}
	return true;
}

bool compiler_push_bytecode_with_value(compiler* comp, opcode op, value v) {
	if (!vector_push_back(uint8_t, &comp->bytecode, (uint8_t) op)) goto FAILURE;
	for (int i = 0; i < 8; i++) {
		if (!vector_push_back(uint8_t, &comp->bytecode, v.bytes[i])) goto FAILURE;
	}
	return true;

FAILURE:
	fputs("error : Bytecode memory allocation faliure\n", stderr);
	return false;
}

bool compiler_push_bytecode_with_null(compiler* comp, opcode op) {
	value v;
	v.u = 0;
	return compiler_push_bytecode_with_value(comp, op, v);
}

bool compiler_push_preproc_token(compiler* comp, char* token) {
	size_t len = strlen(token) + 1;
	char* temp = (char*) malloc(len * sizeof(char));
	if (!temp) goto FAILURE_ALLOC;
	strcpy(temp, token);
	if (!vector_push_back(cctl_ptr(char), &comp->preproc_tokens_vector, temp)) goto FAILURE_VECTOR;
	return true;

FAILURE_ALLOC:
	fputs("error : Preprocessor token memory allocation failure\n", stderr);
	return false;
	
FAILURE_VECTOR:
	fputs("error : Preprocessor tokens vector memory allocation failure\n", stderr);
	free(temp);
	return false;
}