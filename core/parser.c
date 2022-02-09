
#include "parser.h"
#include "../tokens/lit.h"
#include "token.h"
#include <string.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif



#ifdef DEBUG
#include <stdio.h>
#define debugf(...) fprintf(stderr, "!!! " __VA_ARGS__); fprintf(stderr, "\n")
#else
#define debugf (void)sizeof
#endif


#define MODULE_LIST__IMPORT   0
#define MODULE_LIST__EXPORT   1
#define MODULE_LIST__REEXPORT 2


#define STATEMENT__TOP        1
#define STATEMENT__BLOCK      2


static int consume_statement(int);
static int consume_expr(int);
static int consume_expr_group();
static int consume_expr_statement();
static int consume_definition_group();
static int consume_function(int);
static int consume_class(int);
static int consume_expr_zero_many(int);
static int consume_expr_internal(int);
static int consume_definition_list(int, int);
static int consume_destructuring(int);


static int parser_skip = 0;


#define cursor (&(td->curr))
#define peek (&(td->peek))

static inline int a_static_push(int32_t *s) {
  
}

static inline int cursor_next() {
  if (!parser_skip) {
    blep_parser_callback();
  }
  return blep_token_next();
}


#define _STACK_BEGIN(type) { \
  const int _stack_type = type; \
  int _prev_parser_skip = parser_skip; \
  parser_skip = parser_skip || blep_parser_open(type);


#define _STACK_END() ; \
  if (!parser_skip) { blep_parser_close(_stack_type); } \
  parser_skip = _prev_parser_skip; \
}


#define _STACK_END_SEMICOLON() \
    if (cursor->type == TOKEN_SEMICOLON && cursor->special == 0) { \
      cursor_next(); \
    } \
    _STACK_END();

#define _SET_RESTORE() \
  if (!parser_skip) { \
    ++parser_skip; \
    blep_token_set_restore();

#define _RESUME_RESTORE() \
    --parser_skip; \
    blep_token_restore(); \
  }

#define _check(v) { int _ret = v; if (_ret) { return _ret; }};


inline static int consume_basic_key_string_special(int special) {
  if (cursor->type != TOKEN_STRING || (cursor->p[0] == '`' && cursor->len > 1 && cursor->p[cursor->len - 1] != '`')) {
    
    return ERROR__UNEXPECTED;
  }
  cursor->special = special;
  cursor_next();
  return 0;
}


inline static int consume_defn_name(int special) {
  if (cursor->special == LIT_EXTENDS || cursor->type != TOKEN_LIT) {
#ifdef DEBUG
    if (peek->p) {
      debugf("expected empty peek location for empty emit");
      return ERROR__UNEXPECTED;
    }
#endif
    
    if (special && !parser_skip) {
      memcpy(peek, cursor, sizeof(struct token));
      peek->vp = peek->p;  
      cursor->len = 0;
      cursor->special = special;
      cursor->type = TOKEN_SYMBOL;
      cursor_next();
    }
    return 0;
  }

  if (special) {
    
    cursor->type = TOKEN_SYMBOL;  
    cursor->special = special;
    cursor_next();
  } else {
    
    cursor->type = TOKEN_LIT;
    cursor->special = 0;
    cursor_next();
  }
  return 0;
}

static inline int consume_dict(int is_class) {
#ifdef DEBUG
  if (cursor->type != TOKEN_BRACE) {
    debugf("missing open brace for dict");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();

  for (;;) {
    if (cursor->special == MISC_SPREAD) {
      cursor_next();
      _check(consume_expr(0));
      continue;
    }

    
    int is_static = (cursor->special == LIT_STATIC && blep_token_peek() != TOKEN_PAREN);
    if (is_static) {
#if DEBUG
      if (!is_class) {
        debugf("got static inside non-class");
        return ERROR__UNEXPECTED;
      }
#endif
      cursor->type = TOKEN_KEYWORD;
      cursor_next();
    }

    
    if (cursor->special == LIT_ASYNC) {
      int peek_type = blep_token_peek();
      switch (peek_type) {
        case TOKEN_OP:
          if (peek->special != MISC_STAR) {
            break;
          }
          

        case TOKEN_KEYWORD:  
        case TOKEN_LIT:
          cursor->type = TOKEN_KEYWORD;  
          cursor_next();
          break;
      }
    }

    
    if (cursor->special == MISC_STAR) {
      cursor_next();
    }

    
    if ((cursor->special == LIT_GET || cursor->special == LIT_SET) && blep_token_peek() != TOKEN_PAREN) {
      cursor->type = TOKEN_KEYWORD;
      cursor_next();
    }

    
    switch (cursor->type) {
      case TOKEN_SYMBOL:  
      case TOKEN_LIT: {
        cursor->type = TOKEN_LIT;
        cursor->special = SPECIAL__PROPERTY;

        
        if (!is_class) {

          
          switch (blep_token_peek()) {
            case TOKEN_COLON:
            case TOKEN_PAREN:
              break;

            case TOKEN_OP:
              if (peek->special == MISC_EQUALS) {
                break;
              }
              

            default:
              cursor->type = TOKEN_SYMBOL;
          }
        }

        cursor_next();
        break;
      }

      case TOKEN_NUMBER:
        cursor_next();
        break;

      case TOKEN_STRING:
        _check(consume_basic_key_string_special(0));
        break;

      case TOKEN_ARRAY:
        _check(consume_expr_group());
        break;

      default:
        ;
        
    }

    
    switch (cursor->type) {
      case TOKEN_PAREN:
        
        _STACK_BEGIN(STACK__FUNCTION);
        _STACK_BEGIN(STACK__INNER);
        _check(consume_definition_group());
        _check(consume_statement(0));
        _STACK_END();
        _STACK_END();
        break;

      case TOKEN_OP:
        if (cursor->special != MISC_EQUALS) {
          break;
        }
        

      case TOKEN_COLON:
        
        cursor_next();
        

        if (is_class && !is_static) {
          
          
          _STACK_BEGIN(STACK__INNER);
          _check(consume_expr(1));
          _STACK_END();
        } else {
          _check(consume_expr(1));
        }

        break;
    }

    
    switch (cursor->type) {
      case TOKEN_CLOSE:
        cursor_next();
        return 0;

      case TOKEN_EOF:
        
        debugf("got EOF inside dict");
        return ERROR__UNEXPECTED;

      case TOKEN_OP:
        if (cursor->special == MISC_COMMA) {
          cursor_next();
          continue;
        }
        if (cursor->special == MISC_STAR) {
          continue;  
        }
        break;

      case TOKEN_SEMICOLON:
        cursor_next();
        continue;

      case TOKEN_SYMBOL:  
      case TOKEN_LIT:
      case TOKEN_NUMBER:
      case TOKEN_STRING:
      case TOKEN_ARRAY:
        continue;
    }

    debugf("unknown left-side dict part: %d", cursor->type);
    return ERROR__UNEXPECTED;
  }
}



static int consume_expr_zero_many(int is_statement) {
  for (;;) {
    _check(consume_expr_internal(is_statement));
    if (cursor->special != MISC_COMMA) {
      break;
    }
    cursor_next();
  }

  return 0;
}


static int consume_expr_group() {
  int open = cursor->type;
#ifdef DEBUG
  switch (cursor->type) {
    case TOKEN_PAREN:
    case TOKEN_ARRAY:
    case TOKEN_TERNARY:
      break;

    default:
      debugf("expected expr group, was: %d", cursor->type);
      return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();
  _check(consume_expr_zero_many(0));

  if (cursor->type != TOKEN_CLOSE) {
    debugf("expected close for expr group (got %d), open was: %d", cursor->type, open);
    return ERROR__UNEXPECTED;
  }
  cursor_next();  
  return 0;
}


static int consume_arrowfunc_from_arrow(int is_statement) {
  if (cursor->special != MISC_ARROW) {
    debugf("arrowfunc missing =>");
    return ERROR__UNEXPECTED;
  }
  cursor_next();  

  if (cursor->type == TOKEN_BRACE) {
    return consume_statement(0);
  }
  _check(consume_expr(is_statement));
  return 0;
}


static int consume_arrowfunc(int is_statement) {
  
  int is_async = (cursor->special == LIT_ASYNC && !(blep_token_peek() == TOKEN_OP && peek->special == MISC_ARROW));
  if (is_async) {
    cursor->type = TOKEN_KEYWORD;
  }

  _STACK_BEGIN(STACK__FUNCTION);

  if (is_async) {
    cursor_next();
  }

  _STACK_BEGIN(STACK__INNER);

  switch (cursor->type) {
    case TOKEN_LIT:
      cursor->type = TOKEN_SYMBOL;
      cursor->special = SPECIAL__DECLARE;
      cursor_next();
      break;

    case TOKEN_PAREN:
      _check(consume_definition_group());
      break;

    default:
      debugf("got unknown part of arrowfunc: %d", cursor->type);
      return ERROR__UNEXPECTED;
  }

  _check(consume_arrowfunc_from_arrow(is_statement));
  _STACK_END();
  _STACK_END();
  return 0;
}

static int consume_template_string() {
#ifdef DEBUG
  if (cursor->type != TOKEN_STRING || cursor->p[0] != '`') {
    debugf("bad templated string");
    return ERROR__UNEXPECTED;
  }
#endif

  for (;;) {
    char end = cursor->p[cursor->len - 1];
    cursor_next();

    if (end == '`') {
      return 0;
    } else if (end != '{') {
      
      
      
      debugf("templated string didn't end with ` or ${");
      return ERROR__UNEXPECTED;
    }

    _check(consume_expr_zero_many(0));

    if (!(cursor->type == TOKEN_STRING && cursor->p[0] == '}')) {
      debugf("templated string didn't restart with }, was %d", cursor->type);
      return ERROR__UNEXPECTED;
    }
  }
}

static int maybe_consume_destructuring() {
  
  switch (cursor->type) {
    case TOKEN_ARRAY:
    case TOKEN_BRACE:
      break;

    default:
      return 0;
  }

  int is_destructuring = 0;

  _SET_RESTORE();
  
  
  is_destructuring = (consume_destructuring(0) == 0) && cursor->special == MISC_EQUALS;
  _RESUME_RESTORE();

  debugf("lookahead got destructuring: %d", is_destructuring);
  if (is_destructuring) {
    return consume_destructuring(0);
  }
  return 0;
}


static int lookahead_is_paren_arrowfunc() {
  if (cursor->special == LIT_ASYNC) {
    cursor_next();
  }

#ifdef DEBUG
  if (cursor->type != TOKEN_PAREN) {
    debugf("internal error, lookahead_is_arrowfunc_paren was not paren: %d", cursor->type);
    return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();

  if (consume_definition_list(0, 0)) {
    return 0;  
  }

  if (cursor->type == TOKEN_CLOSE) {
    cursor_next();
    if (cursor->special == MISC_ARROW) {
      return 1;
    }
  }
  return 0;
}

static int maybe_consume_arrowfunc(int is_statement) {
  
  if (cursor->type == TOKEN_LIT) {
    blep_token_peek();
    if (peek->special == MISC_ARROW) {
      return consume_arrowfunc(is_statement);  
    } else if (cursor->special != LIT_ASYNC) {
      return 0;
    } else if (peek->type == TOKEN_LIT) {
      
      if (peek->special == LIT_FUNCTION) {
        return 0;
      }
      return consume_arrowfunc(is_statement);  
    } else if (peek->type != TOKEN_PAREN) {
      return 0;  
    }
  } else if (cursor->type != TOKEN_PAREN) {
    return 0;
  }
  
  

  if (parser_skip) {
    return 0;  
  }

  int is_arrowfunc = 0;

  _SET_RESTORE();
  is_arrowfunc = lookahead_is_paren_arrowfunc();
  _RESUME_RESTORE();

  debugf("lookahead found arrowfunc=%d", is_arrowfunc);
  if (is_arrowfunc) {
    return consume_arrowfunc(is_statement);
  }

  return 0;
}

static int is_token_assign_like(struct token *t) {
  if (t->special == MISC_EQUALS) {
    return 1;
  }
  int len = t->len;
  return len >= 2 && t->p[len - 1] == '=' && t->p[len - 2] != '=';
}


static int consume_expr_internal(int is_statement) {
  int paren_count = 0;

restart_expr:
  (void)sizeof(0);
  int value_line = 0;
  char *start = cursor->p;

  
  _check(maybe_consume_arrowfunc(is_statement));
  if (start != cursor->p) {
    if (paren_count == 0) {
      
      return 0;
    }
    if (cursor->type != TOKEN_CLOSE && cursor->special != MISC_COMMA) {
      debugf("got bad end after wrapped arrowfunc, type=%d special=%d", cursor->type, cursor->special);
      return ERROR__UNEXPECTED;
    }
  } else {
    
    _check(maybe_consume_destructuring());
    if (start != cursor->p) {
      value_line = cursor->line_no;
    }
  }

#define _maybe_abandon() { if (is_statement && !paren_count) { return 0; } }
#define _transition_to_value() { if (value_line) { _maybe_abandon(); } value_line = cursor->line_no; }

  for (;;) {
    
    switch (cursor->type) {
      case TOKEN_OP:
        if (!value_line && cursor->p[0] == '/') {
          blep_token_update(TOKEN_REGEXP);  
        }
        break;

      case TOKEN_REGEXP:
        if (value_line) {
          blep_token_update(TOKEN_OP);  
        }
        break;

      case TOKEN_LIT:
        cursor->type = TOKEN_SYMBOL;  

        switch (cursor->special) {
          case LIT_ASYNC:
            
            blep_token_peek();
            if (peek->special == LIT_FUNCTION) {
              cursor->type = TOKEN_KEYWORD;
            }
            break;

          case LIT_CLASS:
          case LIT_FUNCTION:
            cursor->type = TOKEN_KEYWORD;
            break;

          case LIT_OF:
            if (value_line && !is_statement) {
              
              cursor->type = TOKEN_OP;
            }
            break;

          case LIT_NEW:
            blep_token_peek();
            if (peek->special != MISC_DOT) {
              
              cursor->type = TOKEN_OP;
            }
            break;

          default:
            if (cursor->special & (_MASK_UNARY_OP | _MASK_REL_OP)) {
              cursor->type = TOKEN_OP;
            } else if (cursor->special & _MASK_KEYWORD) {
              _maybe_abandon();
              cursor->type = TOKEN_KEYWORD;
            }
        }
    }

    
    switch (cursor->type) {
      case TOKEN_KEYWORD:
        _transition_to_value();

        switch (cursor->special) {
          case LIT_ASYNC:
          case LIT_FUNCTION:
            _check(consume_function(0));
            continue;

          case LIT_CLASS:
            _check(consume_class(0));
            continue;
        }

        cursor_next();  
        continue;

      case TOKEN_ARRAY:
        value_line = cursor->line_no;  
        _check(consume_expr_group());
        continue;

      case TOKEN_BRACE:
        _transition_to_value();
        _check(consume_dict(0));
        continue;

      case TOKEN_TERNARY:
        
        _check(consume_expr_group());
        value_line = 0;
        continue;

      case TOKEN_PAREN:
        if (value_line) {
          
          _check(consume_expr_group());
          value_line = cursor->line_no;
          continue;
        }
        ++paren_count;
        cursor_next();

        
        
        if (cursor->type != TOKEN_LIT || cursor->special & _MASK_KEYWORD || blep_token_peek() != TOKEN_CLOSE) {
          goto restart_expr;
        }

        int is_lvalue = 0;
        _SET_RESTORE();

        int paren_remain = paren_count;
        do {
          cursor_next();
          blep_token_peek();
          --paren_remain;
        } while (peek->type == TOKEN_CLOSE && paren_remain);

        blep_token_peek();
        is_lvalue = is_token_assign_like(peek) || peek->special == MISC_INCDEC;
        _RESUME_RESTORE();

        cursor->type = TOKEN_SYMBOL;
        cursor->special = is_lvalue ? SPECIAL__CHANGE : 0;
        cursor_next();
        
        continue;

      case TOKEN_CLOSE:
        if (!paren_count) {
          return 0;
        }
        --paren_count;
        cursor_next();

        
        if (parser_skip && cursor->special == MISC_ARROW) {
          _check(consume_arrowfunc_from_arrow(is_statement));
        }

        value_line = td->line_no;
        continue;

      case TOKEN_STRING:
        if (cursor->p[0] == '}') {
          return 0;  
        } else if (cursor->p[0] == '`') {
          _check(consume_template_string())
          value_line = cursor->line_no;
        } else {
          _transition_to_value();
          cursor_next();
        }
        continue;

      case TOKEN_SYMBOL:
        _transition_to_value();
        cursor->special = 0;  

        blep_token_peek();
        if (is_token_assign_like(peek) || peek->special == MISC_INCDEC) {
          cursor->special = SPECIAL__CHANGE;
        }

        cursor_next();
        continue;

      case TOKEN_NUMBER:
      case TOKEN_REGEXP:
        _transition_to_value();
        cursor_next();
        continue;

      case TOKEN_OP:
        break;  

      default:
        return 0;
    }
#ifdef DEBUG
    if (cursor->type != TOKEN_OP) {
      debugf("non-op fell through");
      return ERROR__INTERNAL;
    }
#endif

    

    if (cursor->special & _MASK_UNARY_OP) {
      if (start != cursor->p && value_line) {
        
        _maybe_abandon();
      }

      if (cursor->special == LIT_YIELD) {
        int line_no = cursor->line_no;
        cursor_next();
        if (cursor->line_no != line_no) {
          _maybe_abandon();  
        }
      } else {
        cursor_next();
      }

      value_line = 0;
      continue;
    } else if (is_token_assign_like(cursor)) {
      
      cursor_next();
      goto restart_expr;
    }

    switch (cursor->special) {
      case LIT_YIELD:
        cursor_next();

      case MISC_ARROW:
        
        cursor_next();
        if (cursor->type == TOKEN_BRACE) {
          _check(consume_statement(0));
        }
        goto restart_expr;

      case MISC_COMMA:
        if (paren_count) {
          cursor_next();
          goto restart_expr;
        }
        return 0;

      case MISC_CHAIN:
      case MISC_DOT:
        if (!value_line) {
          _maybe_abandon();
        }
        cursor_next();

        
        
        if (cursor->type == TOKEN_PAREN || cursor->type == TOKEN_ARRAY) {
          value_line = cursor->line_no;
          continue;
        } else if (cursor->type != TOKEN_LIT) {
          debugf("got dot/chain with unknown after: %d", cursor->type);
          return ERROR__UNEXPECTED;
        }
        cursor->special = SPECIAL__PROPERTY;
        cursor_next();
        continue;

      case MISC_INCDEC:
        if (value_line) {
          if (cursor->line_no != value_line) {
            _maybe_abandon();  
          }
          cursor_next();  
          continue;
        }

        int paren_count_here = 0;

        
        cursor_next();
        while (cursor->type == TOKEN_PAREN) {
          ++paren_count_here;
          cursor_next();
        }
        paren_count += paren_count_here;
        if (cursor->type != TOKEN_LIT) {
          continue;  
        }

        blep_token_peek();
        if (peek->type == TOKEN_CLOSE) {
          
        } else if (paren_count_here) {
          
          continue;
        } else if (peek->special == MISC_DOT || peek->special == MISC_CHAIN || peek->type == TOKEN_PAREN || peek->type == TOKEN_ARRAY) {
          
          
          continue;
        }

        cursor->type = TOKEN_SYMBOL;
        cursor->special = SPECIAL__CHANGE;
        value_line = cursor->line_no;
        cursor_next();
        continue;

      default:
        
        value_line = 0;
        cursor_next();
    }
  }

#undef _maybe_abandon
#undef _transition_to_value
}

static inline int consume_expr(int is_statement) {
  char *start = cursor->p;
  _check(consume_expr_internal(is_statement));

  if (start == cursor->p) {
    debugf("could not consume expr, was: %d (%.*s)", cursor->type, cursor->len, cursor->p);
    return ERROR__UNEXPECTED;
  }
  return 0;
}



static int consume_destructuring(int special) {
#ifdef DEBUG
  int special_mask = (SPECIAL__TOP | SPECIAL__DECLARE);
  if ((special | special_mask) != special_mask) {
    debugf("destrucuring special should only be TOP | DECLARE, was %d", special);
    return ERROR__INTERNAL;
  }
  if (cursor->type != TOKEN_BRACE && cursor->type != TOKEN_ARRAY) {
    debugf("destructuring did not start with { or [");
    return ERROR__UNEXPECTED;
  }
#endif
  int start = cursor->type;
  cursor->special = SPECIAL__DESTRUCTURING;
  cursor_next();

  for (;;) {
    switch (cursor->type) {
      case TOKEN_CLOSE:
        cursor_next();
        return 0;

      case TOKEN_SYMBOL:  
      case TOKEN_LIT: {
        
        if (blep_token_peek() == TOKEN_COLON) {
          
          cursor->special = SPECIAL__PROPERTY;
        } else {
          
          cursor->type = TOKEN_SYMBOL;
          cursor->special = SPECIAL__PROPERTY | SPECIAL__CHANGE | special;
        }
        cursor_next();
        break;
      }

      case TOKEN_STRING:
        _check(consume_basic_key_string_special(0));

        if (cursor->type != TOKEN_COLON) {
          debugf("destructuring had string with no trailing colon");
          return ERROR__UNEXPECTED;
        }
        break;

      case TOKEN_ARRAY:
        if (start == TOKEN_BRACE) {
          
          _check(consume_expr_group());
          break;
        }
        _check(consume_destructuring(special));
        break;

      case TOKEN_BRACE:
        
        _check(consume_destructuring(special));
        break;

      case TOKEN_OP:
        if (cursor->special == MISC_COMMA) {
          
          cursor_next();
          continue;
        }
        if (cursor->special == MISC_SPREAD) {
          
          cursor_next();
          continue;
        }
        

      default:
        debugf("got unexpected inside object destructuring: type=%d, special=%d", cursor->type, cursor->special);
        return ERROR__UNEXPECTED;
    }

    
    if (cursor->type == TOKEN_COLON) {
      cursor_next();

      switch (cursor->type) {
        case TOKEN_ARRAY:
        case TOKEN_BRACE:
          _check(consume_destructuring(special));
          break;

        case TOKEN_SYMBOL:  
        case TOKEN_LIT:
          cursor->type = TOKEN_SYMBOL;
          cursor->special = SPECIAL__CHANGE | special;
          cursor_next();
          break;
      }
    }

    
    if (cursor->special == MISC_EQUALS) {
      cursor_next();
      _check(consume_expr(0));
    }
  }
}


static int consume_optional_definition(int special, int is_statement) {
  int is_spread = 0;
  int is_assign = 0;

  
  if (cursor->special == MISC_SPREAD) {
    cursor_next();
    is_spread = 1;
  }

  switch (cursor->type) {
    case TOKEN_SYMBOL:  
    case TOKEN_LIT:
      
      cursor->type = TOKEN_SYMBOL;
      cursor->special = SPECIAL__DECLARE | special;

      
      blep_token_peek();
      switch (peek->special) {
        case LIT_IN:
        case LIT_OF:
        case MISC_EQUALS:
          cursor->special |= SPECIAL__CHANGE;
      }
      cursor_next();
      break;

    case TOKEN_BRACE:
    case TOKEN_ARRAY:
      _check(consume_destructuring(special | SPECIAL__DECLARE));
      break;

    default:
      if (is_spread) {
        debugf("destructuring had bad spread");
        return ERROR__UNEXPECTED;
      }
      return 0;  
  }

  return 0;
}


static int consume_optional_assign_suffix(int is_statement) {
  if (cursor->special == MISC_EQUALS) {
    cursor_next();
    _STACK_BEGIN(STACK__EXPR);
    _check(consume_expr(is_statement));
    _STACK_END();
  }
  return 0;
}


static int consume_definition_list(int special, int is_statement) {
  for (;;) {
    _check(consume_optional_definition(special, is_statement));
    _check(consume_optional_assign_suffix(is_statement));
    if (cursor->special != MISC_COMMA) {
      return 0;
    }
    cursor_next();
  }
}



static int consume_definition_group() {
  if (cursor->type != TOKEN_PAREN) {
    debugf("definition didn't start with paren, was type=%d special=%d", cursor->type, cursor->special);
    return ERROR__UNEXPECTED;
  }
  cursor_next();

  if (cursor->type != TOKEN_CLOSE) {
    _check(consume_definition_list(SPECIAL__TOP, 0));

    if (cursor->type != TOKEN_CLOSE) {
      debugf("arg_group did not finish with close");
      return ERROR__UNEXPECTED;
    }
  }
  cursor_next();
  return 0;
}

static int consume_function(int special) {
  cursor->type = TOKEN_KEYWORD;

  
  _STACK_BEGIN(STACK__FUNCTION);

  int is_async = 0;
  int is_generator = 0;

  if (cursor->special == LIT_ASYNC) {
    is_async = 1;
    cursor_next();
    cursor->type = TOKEN_KEYWORD;
  }

  if (cursor->special != LIT_FUNCTION) {
    debugf("function did not start with 'function'");
    return ERROR__UNEXPECTED;
  }
  cursor_next();

  if (cursor->special == MISC_STAR) {
    is_generator = 1;
    cursor_next();
  }

  _check(consume_defn_name(special));
  debugf("function, generator=%d async=%d", is_generator, is_async);

  _STACK_BEGIN(STACK__INNER);
  _check(consume_definition_group());
  _check(consume_statement(0));
  _STACK_END();

  _STACK_END();
  return 0;
}

static int consume_class(int special) {
#ifdef DEBUG
  if (cursor->special != LIT_CLASS) {
    debugf("expected class keyword");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor->type = TOKEN_KEYWORD;
  _STACK_BEGIN(STACK__CLASS);
  cursor_next();

  _check(consume_defn_name(special));

  if (cursor->special == LIT_EXTENDS) {
    cursor->type = TOKEN_KEYWORD;
    cursor_next();

    
    
    _STACK_BEGIN(STACK__EXPR);
    _check(consume_expr(1));  
    _STACK_END();
  }

  _check(consume_dict(1));
  _STACK_END();
  return 0;
}

static int consume_decl_stack(int special) {
#ifdef DEBUG
  if (!(cursor->special & _MASK_DECL)) {
    debugf("expected decl start");
    return ERROR__UNEXPECTED;
  }
#endif
  _STACK_BEGIN(STACK__DECLARE);
  special |= (cursor->special == LIT_VAR ? SPECIAL__TOP : 0);
  cursor->type = TOKEN_KEYWORD;
  cursor_next();
  _check(consume_definition_list(special, 1));
  _STACK_END_SEMICOLON();
  return 0;
}


static int consume_module_list_deep(int mode) {
#ifdef DEBUG
  if (cursor->type != TOKEN_BRACE) {
    debugf("expected { to start module deep");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();

  for (;;) {
    switch (cursor->type) {
      case TOKEN_CLOSE:
        cursor_next();
        return 0;

      case TOKEN_SYMBOL:  
      case TOKEN_LIT:
        break;

      case TOKEN_OP:
        if (cursor->special == MISC_COMMA) {
          cursor_next();
          continue;
        }
        

      default:
        debugf("deep module list expected lit or close");
        return ERROR__UNEXPECTED;
    }

    blep_token_peek();
    if (peek->special == LIT_AS) {
      
      

      if (mode == MODULE_LIST__EXPORT) {
        
        cursor->special = 0;
        cursor->type = TOKEN_SYMBOL;
      } else {
        
        cursor->special = SPECIAL__EXTERNAL;
        cursor->type = TOKEN_LIT;  
      }
      cursor_next();

      
      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      if (cursor->type != TOKEN_LIT && cursor->type != TOKEN_SYMBOL) {
        debugf("missing literal after 'as'");
        return ERROR__UNEXPECTED;
      }

      if (mode == MODULE_LIST__IMPORT) {
        
        cursor->type = TOKEN_SYMBOL;
        cursor->special = SPECIAL__DECLARE | SPECIAL__TOP;
      } else {
        
        cursor->type = TOKEN_LIT;
        cursor->special = SPECIAL__EXTERNAL;
      }
      cursor_next();

    } else {
      
      switch (mode) {
        case MODULE_LIST__EXPORT:
          
          
          cursor->type = TOKEN_SYMBOL;
          cursor->special = SPECIAL__EXTERNAL;
          break;

        case MODULE_LIST__REEXPORT:
          
          cursor->type = TOKEN_LIT;
          cursor->special = SPECIAL__EXTERNAL;
          break;

        default:
          
          cursor->type = TOKEN_SYMBOL;
          cursor->special = SPECIAL__EXTERNAL | SPECIAL__DECLARE | SPECIAL__TOP;
      }
      cursor_next();

    }

    
    if (cursor->special == MISC_COMMA) {
      cursor_next();
    }
  }
}


static int consume_import_module_list() {
  for (;;) {
    
    if (cursor->type == TOKEN_BRACE) {
      _check(consume_module_list_deep(MODULE_LIST__IMPORT));
      if (cursor->special != MISC_COMMA) {
        return 0;
      }
      cursor_next();
      continue;
    }

    switch (cursor->type) {
      case TOKEN_OP:
        if (cursor->special == MISC_COMMA) {
          cursor_next();
          continue;
        }

        
        if (cursor->special != MISC_STAR) {
          return 0;
        }
        cursor_next();

        if (cursor->special != LIT_AS) {
          debugf("expected `* as ...`");
          return ERROR__UNEXPECTED;
        }
        cursor->type = TOKEN_KEYWORD;
        cursor_next();

        if (cursor->type != TOKEN_LIT && cursor->type != TOKEN_SYMBOL) {
          debugf("missing literal after 'import * as'");
          return ERROR__UNEXPECTED;
        }
        cursor->type = TOKEN_SYMBOL;
        cursor->special = SPECIAL__DECLARE | SPECIAL__TOP;
        cursor_next();
        break;

      case TOKEN_SYMBOL:  
      case TOKEN_LIT:
        
        cursor->type = TOKEN_SYMBOL;
        cursor->special = SPECIAL__DECLARE | SPECIAL__TOP;
        cursor_next();
        break;

      default:
        return 0;
    }

    if (cursor->special != MISC_COMMA) {
      return 0;
    }
    cursor_next();
  }
}

static int consume_import() {
#ifdef DEBUG
  if (cursor->special != LIT_IMPORT) {
    debugf("missing import keyword");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor->type = TOKEN_KEYWORD;
  cursor_next();

  if (cursor->type != TOKEN_STRING) {
    _check(consume_import_module_list());

    
    if (cursor->special != LIT_FROM) {
      debugf("missing from keyword");
      return ERROR__UNEXPECTED;
    }
    cursor->type = TOKEN_KEYWORD;
    cursor_next();
  }

  
  return consume_basic_key_string_special(SPECIAL__EXTERNAL);
}


static int consume_export_reexport() {
#ifdef DEBUG
  if (cursor->special != LIT_EXPORT) {
    debugf("missing export keyword");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();  

  switch (cursor->type) {
    case TOKEN_BRACE:
      _check(consume_module_list_deep(MODULE_LIST__REEXPORT));
      break;

    case TOKEN_OP:
      if (cursor->special != MISC_STAR) {
        debugf("expected `export *`");
        return ERROR__UNEXPECTED;
      }
      cursor_next();

      if (cursor->special == LIT_AS) {
        cursor->type = TOKEN_KEYWORD;
        cursor_next();

        if (cursor->type != TOKEN_LIT && cursor->type != TOKEN_SYMBOL) {
          debugf("reexport * as foo missing foo");
          return ERROR__UNEXPECTED;
        }

        cursor->type = TOKEN_LIT;
        cursor->special = SPECIAL__EXTERNAL;
        cursor_next();
      }
      break;

    default:
      debugf("unexpected after export");
      return ERROR__UNEXPECTED;
  }

  if (cursor->special != LIT_FROM) {
    debugf("could not find FROM again");
    return ERROR__INTERNAL;
  }
  cursor->type = TOKEN_KEYWORD;
  cursor_next();
  return consume_basic_key_string_special(SPECIAL__EXTERNAL);
}


static int consume_export_declare() {
#ifdef DEBUG
  if (cursor->special != LIT_EXPORT) {
    debugf("missing export keyword");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor_next();  

  int special_hoist;
  int is_default = (cursor->special == LIT_DEFAULT);
  if (is_default) {
    cursor->type = TOKEN_KEYWORD;
    cursor_next();  
    special_hoist = SPECIAL__DECLARE | SPECIAL__CHANGE | SPECIAL__DEFAULT;
  } else {
    special_hoist = SPECIAL__DECLARE | SPECIAL__CHANGE | SPECIAL__EXTERNAL;
  }

  switch (cursor->special) {
    case LIT_CLASS:
      _check(consume_class(special_hoist));
      return 0;

    case LIT_ASYNC:
      blep_token_peek();
      if (peek->special != LIT_FUNCTION) {
        break;  
      }
      

    case LIT_FUNCTION:
      _check(consume_function(special_hoist));
      return 0;
  }

  if (is_default) {
    return consume_expr_statement();  
  } else if (cursor->special & _MASK_DECL) {
    return consume_decl_stack(SPECIAL__EXTERNAL);
  }

  debugf("bad `export` declaration (should be default, var/lit/const, function, class)");
  return ERROR__UNEXPECTED;
}


static int consume_export_wrap() {
#ifdef DEBUG
  if (cursor->special != LIT_EXPORT) {
    debugf("missing export keyword");
    return ERROR__UNEXPECTED;
  }
#endif
  cursor->type = TOKEN_KEYWORD;  

  
  blep_token_peek();

  int is_reexport = 0;
  if (peek->special == MISC_STAR) {
    is_reexport = 1;  
  } else if (peek->type == TOKEN_BRACE) {
    _SET_RESTORE();
    cursor_next();  
    _check(consume_module_list_deep(MODULE_LIST__EXPORT));
    is_reexport = (cursor->special == LIT_FROM);
    _RESUME_RESTORE();

    if (!is_reexport) {
      _STACK_BEGIN(STACK__MODULE);
      cursor_next();
      _check(consume_module_list_deep(MODULE_LIST__EXPORT));
      _STACK_END_SEMICOLON();
      return 0;
    }
  }

  if (is_reexport) {
    _STACK_BEGIN(STACK__MODULE);
    _check(consume_export_reexport());
    _STACK_END_SEMICOLON();
  } else {
    _STACK_BEGIN(STACK__EXPORT);
    _check(consume_export_declare());
    _STACK_END_SEMICOLON();
  }

  return 0;
}

static inline int consume_control_group_inner(int control_hash) {
  switch (control_hash) {
    case LIT_CATCH:
      
      return consume_optional_definition(0, 0);

    case LIT_AWAIT:
    case LIT_FOR:
      break;

    default:
      if (cursor->type != TOKEN_CLOSE) {
        _STACK_BEGIN(STACK__EXPR);
        _check(consume_expr_zero_many(0));
        _STACK_END();
      }
      return 0;
  }

  if (cursor->type == TOKEN_SEMICOLON) {
    
  } else {
    if (cursor->special & _MASK_DECL) {
      int allow_semicolon = (control_hash == LIT_FOR);
      _STACK_BEGIN(STACK__DECLARE);

      
      int special = cursor->special == LIT_VAR ? SPECIAL__TOP : 0;
      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      char *start = cursor->p;
      _check(consume_optional_definition(special, 0));
      if (start == cursor->p) {
        debugf("expected var def after decl");
        return ERROR__UNEXPECTED;
      }

      
      if (cursor->special == LIT_OF || cursor->special == LIT_IN) {
        cursor->type = TOKEN_OP;
        cursor_next();
        _STACK_BEGIN(STACK__EXPR);
        _check(consume_expr(0));
        _STACK_END();
        allow_semicolon = 0;
      } else {
        
        
        _check(consume_optional_assign_suffix(0));
        if (cursor->special == MISC_COMMA) {
          cursor_next();
          _check(consume_definition_list(special, 0));
        }
      }
      _STACK_END();
      if (!allow_semicolon) {
        return 0;
      }

    } else {
      
      
      _check(consume_expr_zero_many(0));
    }
  }

  
  if (control_hash == LIT_AWAIT) {
    return 0;
  }

  
  if (cursor->type != TOKEN_SEMICOLON) {
    return 0;  
  }
  cursor_next();

  
  if (cursor->type != TOKEN_SEMICOLON) {
    _STACK_BEGIN(STACK__EXPR);
    _check(consume_expr_zero_many(0));
    _STACK_END();
  }
  if (cursor->type != TOKEN_SEMICOLON) {
    debugf("expected 2nd semicolon");
    return ERROR__UNEXPECTED;
  }
  cursor_next();

  
  if (cursor->type == TOKEN_CLOSE) {
    return 0;
  }
  _STACK_BEGIN(STACK__EXPR);
  _check(consume_expr_zero_many(0));
  _STACK_END();
  return 0;
}

static int consume_control() {
#ifdef DEBUG
  if (!(cursor->special & _MASK_CONTROL)) {
    debugf("expected _MASK_CONTROL for consume_control");
    return ERROR__INTERNAL;
  }
#endif

  int control_hash = cursor->special;
  int consume_paren = (control_hash & _MASK_CONTROL_PAREN);

  _STACK_BEGIN(STACK__CONTROL);
  cursor->type = TOKEN_KEYWORD;
  cursor_next();

  
  if (control_hash == LIT_FOR) {
    if (cursor->special == LIT_AWAIT) {
      control_hash = LIT_AWAIT;
      cursor->type = TOKEN_KEYWORD;
      cursor_next();
    }
  }

  
  if (consume_paren && cursor->type == TOKEN_PAREN) {
    cursor_next();
    _check(consume_control_group_inner(control_hash));
    if (cursor->type != TOKEN_CLOSE) {
      debugf("could not find closer of control ()");
      return ERROR__UNEXPECTED;
    }
    cursor_next();
  }

  
  if (control_hash == LIT_DO) {
    _check(consume_statement(0));

    
    
    
    char prev = cursor->vp[-1];
    if (prev != ';' && cursor->type == TOKEN_SEMICOLON) {
      cursor_next();
    }

    if (cursor->special != LIT_WHILE) {
      debugf("could not find while of do-while");
      return ERROR__UNEXPECTED;
    }
    cursor->type = TOKEN_KEYWORD;
    cursor_next();

    if (cursor->type != TOKEN_PAREN) {
      debugf("could not find paren for while");
      return ERROR__UNEXPECTED;
    }

    
    _check(consume_expr_group());

    
    if (cursor->type == TOKEN_SEMICOLON) {
      cursor_next();
    }

  } else {
    
     _check(consume_statement(0));
  }

  _STACK_END();
  return 0;
}

static int consume_expr_statement() {
  _STACK_BEGIN(STACK__EXPR);

  char *start = cursor->p;
  _check(consume_expr_zero_many(1));
  if (start == cursor->p) {
    debugf("could not consume any expr statement, token=%d %.*s", cursor->type, cursor->len, cursor->p);
    return ERROR__UNEXPECTED;
  }

  _STACK_END_SEMICOLON();
  return 0;
}

static int consume_statement(int mode) {
  switch (cursor->type) {
    case TOKEN_EOF:
    case TOKEN_COLON:
      debugf("got EOF/COLON for statement");
      return ERROR__UNEXPECTED;

    case TOKEN_CLOSE:
      return 0;

    case TOKEN_BRACE:
      
      cursor->type = TOKEN_BLOCK;
      _STACK_BEGIN(STACK__BLOCK);
      cursor_next();

      do {
        _check(consume_statement(STATEMENT__BLOCK));
      } while (cursor->type != TOKEN_CLOSE);

      cursor->special = TOKEN_BLOCK;
      cursor_next();
      _STACK_END();
      return 0;

    case TOKEN_SEMICOLON:
      _STACK_BEGIN(STACK__MISC);
      cursor_next();
      _STACK_END();
      return 0;

    case TOKEN_LABEL:  
      _STACK_BEGIN(STACK__LABEL);
      cursor_next();

      if (cursor->type != TOKEN_COLON) {
        return ERROR__UNEXPECTED;
      }
      cursor_next();
      _check(consume_statement(0));

      _STACK_END();
      return 0;

    case TOKEN_KEYWORD:  
    case TOKEN_SYMBOL:   
    case TOKEN_LIT:
      break;

    default:
      return consume_expr_statement();
  }

  switch (cursor->special) {
    case LIT_DEFAULT:
      _STACK_BEGIN(STACK__LABEL);

      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      if (cursor->type != TOKEN_COLON) {
        debugf("no : after default");
        return ERROR__UNEXPECTED;
      }
      cursor_next();
      

      _STACK_END();
      return 0;

    case LIT_CASE:
      _STACK_BEGIN(STACK__LABEL);

      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      _STACK_BEGIN(STACK__EXPR);
      _check(consume_expr(0));
      _STACK_END();

      if (cursor->type != TOKEN_COLON) {
        debugf("no : after case");
        return ERROR__UNEXPECTED;
      }
      cursor_next();
      

      _STACK_END();
      return 0;

    case LIT_RETURN:
    case LIT_THROW:
      _STACK_BEGIN(STACK__MISC);
      int line_no = cursor->line_no;

      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      if (line_no == cursor->line_no && cursor->type != TOKEN_SEMICOLON) {
        _STACK_BEGIN(STACK__EXPR);
        _check(consume_expr_zero_many(1));
        _STACK_END();
      }

      _STACK_END_SEMICOLON();
      return 0;

    case LIT_DEBUGGER:
      _STACK_BEGIN(STACK__MISC);

      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      _STACK_END_SEMICOLON();
      return 0;

    case LIT_CONTINUE:
    case LIT_BREAK:
      _STACK_BEGIN(STACK__MISC);
      int line_no = cursor->line_no;

      cursor->type = TOKEN_KEYWORD;
      cursor_next();

      if (line_no == cursor->line_no) {
        if (cursor->type == TOKEN_LIT) {
          cursor->special = 0;
          cursor->type = TOKEN_LABEL;
          cursor_next();
        }
      }

      _STACK_END_SEMICOLON();
      return 0;

    case LIT_ASYNC:
      blep_token_peek();
      if (peek->special != LIT_FUNCTION) {
        break;  
      }
      

    case LIT_FUNCTION:
      if (!mode) {
        return consume_expr_statement();
      }
      return consume_function(SPECIAL__DECLARE | SPECIAL__CHANGE);

    case LIT_CLASS:
      if (!mode) {
        return consume_expr_statement();
      }
      return consume_class(SPECIAL__DECLARE | SPECIAL__CHANGE);

    case LIT_IMPORT:
      
      blep_token_peek();
      if (peek->type == TOKEN_PAREN || peek->special == MISC_DOT) {
        return consume_expr_statement();
      }
      if (mode == STATEMENT__TOP) {
        _STACK_BEGIN(STACK__MODULE);
        _check(consume_import());
        _STACK_END_SEMICOLON();
        return 0;
      }
      break;

    case LIT_EXPORT:
      if (mode == STATEMENT__TOP) {
        return consume_export_wrap();
      }
      break;
  }

  if (!(cursor->special & _MASK_MASQUERADE)) {
    if (blep_token_peek() == TOKEN_COLON) {
      
      
      cursor->special = 0;
      cursor->type = TOKEN_LABEL;
      return consume_statement(0);
    }
  }

  if (cursor->special & _MASK_CONTROL) {
    return consume_control();
  } else if (cursor->special & _MASK_DECL) {
    return consume_decl_stack(0);
  } else if (cursor->special & _MASK_UNARY_OP || !cursor->special) {
    return consume_expr_statement();
  }

  
  
  if (cursor->special & _MASK_KEYWORD) {
    _STACK_BEGIN(STACK__MISC);
    cursor->type = TOKEN_KEYWORD;
    cursor_next();
    _STACK_END_SEMICOLON();
    return 0;
  }

  return consume_expr_statement();
}

EMSCRIPTEN_KEEPALIVE
int blep_parser_init(char *p, int len) {
  _check(blep_token_init(p, len));
  parser_skip = 0;

  if (p[0] == '#' && p[1] == '!') {
    td->at = memchr(p, '\n', td->end - p);
    if (td->at == NULL) {
      td->at = p + len;
    }
    blep_token_peek();
    peek->vp = p;
  }

  blep_token_next();
  return 0;
}

EMSCRIPTEN_KEEPALIVE
int blep_parser_run() {
  if (cursor->type == TOKEN_EOF) {
    return 0;
  }
  char *head = cursor->p;

  _check(consume_statement(STATEMENT__TOP));

  int len = cursor->p - head;
  if (len == 0 && cursor->type != TOKEN_EOF) {
    debugf("blep_parser_run consumed nothing, token=%d", cursor->type);
    return ERROR__UNEXPECTED;
  }
  return len;
}

EMSCRIPTEN_KEEPALIVE
struct token *blep_parser_cursor() {
  return cursor;
}
