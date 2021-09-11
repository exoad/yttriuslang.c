
#include "../core/token.h"
#include "../core/parser.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

typedef struct _testdef
{
  const char *name;
  const char *input;
  int *expected; 
  int is_module;
  struct testdef *next; 
} testdef;

static struct token *t;
static int render_output = 0;

struct
{
  testdef *def;
  int at;
  int len;
  int error;
} active;

void blep_parser_callback()
{
  int actual = t->type;
  int expected = -1;

  if (active.at < active.len)
  {
    expected = active.def->expected[active.at];
  }
  else if (active.at == active.len)
  {
    expected = 0;
  }

  if (actual != expected)
  {
    if (render_output)
    {
      printf("%d: actual=%d expected=%d `%.*s`\n", active.at, actual, expected, t->len, t->p);
    }
    active.error = 1;
  }
  else if (render_output)
  {
    printf("%d: ok=%d `%.*s`\n", active.at, actual, t->len, t->p);
  }
  ++active.at;
}

int blep_parser_open(int type)
{
  return 0;
}

void blep_parser_close(int type)
{
  
}

int run_testdef(testdef *def)
{
  t = blep_parser_cursor();

  active.def = def;
  active.at = 0;
  active.len = 0;
  active.error = 0;

  
  while (def->expected[active.len])
  {
    ++active.len;
  }

  if (render_output)
  {
    printf(">> %s\n", def->name);
  }

  int ret = blep_parser_init((char *)def->input, strlen(def->input));
  if (ret >= 0)
  {
    do
    {
      ret = blep_parser_run();
    } while (ret > 0);
  }

  if (ret)
  {
    if (render_output)
    {
      printf("ERROR: internal error (%d)\n", ret);
    }
    return ret;
  }
  else if (active.at != active.len)
  {
    if (render_output)
    {
      printf("ERROR: mismatched length, actual=%d expected=%d\n", active.at, active.len);
    }
    return 1;
  }
  else if (active.error)
  {
    if (render_output)
    {
      printf("ERROR\n");
    }
    return active.error;
  }

  if (render_output)
  {
    printf("OK!\n");
  }
  return 0;
}
#define _test(_name, _input, ...)           \
  {                                         \
    testdef tdef;                           \
    tdef.name = _name;                      \
    tdef.input = _input;                    \
    tdef.is_module = _name[0] == '^';       \
    tdef.next = NULL;                       \
    int v[] = {__VA_ARGS__ TOKEN_EOF};      \
    tdef.expected = v;                      \
    int lerr = run_testdef(&tdef);          \
    if (lerr)                               \
    {                                       \
      err |= lerr;                          \
      last->next = malloc(sizeof(testdef)); \
      last = (testdef *)last->next;         \
      *last = tdef;                         \
      ++ecount;                             \
    }                                       \
    if (render_output)                      \
    {                                       \
      printf("\n");                         \
    }                                       \
    ++count;                                \
  }

int main()
{
  int err = 0;
  int count = 0;
  int ecount = 0;
  testdef fail, *last = &fail;
  bzero(&fail, sizeof(fail));

  _test("zero", "\n");

  _test("single symbol", "foo",
        TOKEN_SYMBOL, 
  );

  _test("single comment", "blah",
        TOKEN_SYMBOL, 
        TOKEN_NUMBER, 
  );

  _test("simple", "var x = 1;",
        TOKEN_KEYWORD,   
        TOKEN_SYMBOL,    
        TOKEN_OP,        
        TOKEN_NUMBER,    
        TOKEN_SEMICOLON, 
  );

  _test("ternary", "a ? : x\nfoo:\n?:",
        TOKEN_SYMBOL,  
        TOKEN_TERNARY, 
        TOKEN_CLOSE,   
        TOKEN_SYMBOL,  
        TOKEN_LABEL,   
        TOKEN_COLON,   
        TOKEN_TERNARY, 
        TOKEN_CLOSE,   
  );

  _test("let is always keyword in strict", "+let",
        TOKEN_OP,      
        TOKEN_KEYWORD, 
  );

  _test("always prefer keyword", "x = if (a) /123/",
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_REGEXP,  
  );

  _test("control keyword starts new statement on newline", "x =\n if (a) /123/",
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_REGEXP,  
  );

  _test("function decl regexp", "function foo(y) {} / 100 /",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_PAREN,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_REGEXP,  
  );

  _test("class decl regexp", "class {} / 100 /",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_BRACE,   
        TOKEN_CLOSE,   
        TOKEN_REGEXP,  
  );

  _test("function statement", "(function(y) {} / 100 /)",
        TOKEN_PAREN,   
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_NUMBER,  
        TOKEN_OP,      
        TOKEN_CLOSE,   
  );

  _test("simple async arrow function", "async () => await /123/",
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_OP,      
        TOKEN_REGEXP,  
  );

  _test("async arrow function", "() => async () => await\n/123/\nawait /1/",
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_OP,      
        TOKEN_REGEXP,  
        TOKEN_OP,      
        TOKEN_REGEXP,  
  );

  _test("class statement", "x = class Foo extends {} { if(x) {} } /123/",
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_LIT,     
        TOKEN_KEYWORD, 
        TOKEN_BRACE,   
        TOKEN_CLOSE,   
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_NUMBER,  
        TOKEN_OP,      
  );

  _test("dict string name", "+{'abc'() {}}",
        TOKEN_OP,     
        TOKEN_BRACE,  
        TOKEN_STRING, 
        TOKEN_PAREN,  
        TOKEN_CLOSE,  
        TOKEN_BLOCK,  
        TOKEN_CLOSE,  
        TOKEN_CLOSE,  
  );

  _test("dict after comma", ",{}",
        TOKEN_OP,    
        TOKEN_BRACE, 
        TOKEN_CLOSE, 
  );

  
  _test("ASI for PostfixExpression", "a\n++\nb",
        TOKEN_SYMBOL, 
        TOKEN_OP,     
        TOKEN_SYMBOL, 
  );

  _test("dict keyword-ness", "void {async * get get() {}}",
        TOKEN_OP,      
        TOKEN_BRACE,   
        TOKEN_KEYWORD, 
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("yield is op", "function*() { yield /123/ }",
        TOKEN_KEYWORD, 
        TOKEN_OP,      
        TOKEN_SYMBOL,  
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_OP,      
        TOKEN_REGEXP,  
        TOKEN_CLOSE,   
  );

  _test("ASI rule for yield is ignored in group", "function*() { (yield\n/123/) }",
        TOKEN_KEYWORD, 
        TOKEN_OP,      
        TOKEN_SYMBOL,  
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_PAREN,   
        TOKEN_OP,      
        TOKEN_REGEXP,  
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  
  _test("for() matches let keyword", "for(let x;let;);",
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_KEYWORD,   
        TOKEN_SYMBOL,    
        TOKEN_SEMICOLON, 
        TOKEN_KEYWORD,   
        TOKEN_SEMICOLON, 
        TOKEN_CLOSE,     
        TOKEN_SEMICOLON, 
  );

  _test("for await() matches keyword", "for await(let x);",
        TOKEN_KEYWORD,   
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_KEYWORD,   
        TOKEN_SYMBOL,    
        TOKEN_CLOSE,     
        TOKEN_SEMICOLON, 
  );

  _test("for(blah of foo) matches keyword", "for(const x of bar);",
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_KEYWORD,   
        TOKEN_SYMBOL,    
        TOKEN_OP,        
        TOKEN_SYMBOL,    
        TOKEN_CLOSE,     
        TOKEN_SEMICOLON, 
  );

  
  _test("do-while while sanity check", "do while(2) x\nwhile(1) while(0);",
        TOKEN_KEYWORD,   
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_NUMBER,    
        TOKEN_CLOSE,     
        TOKEN_SYMBOL,    
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_NUMBER,    
        TOKEN_CLOSE,     
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_NUMBER,    
        TOKEN_CLOSE,     
        TOKEN_SEMICOLON, 
  );

  _test("do-while inside control", "if do ; while(0) bar",
        TOKEN_KEYWORD,   
        TOKEN_KEYWORD,   
        TOKEN_SEMICOLON, 
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_NUMBER,    
        TOKEN_CLOSE,     
        TOKEN_SYMBOL,    
  );

  _test("do-while block", "do {} while ();",
        TOKEN_KEYWORD,   
        TOKEN_BLOCK,     
        TOKEN_CLOSE,     
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_CLOSE,     
        TOKEN_SEMICOLON, 
  );

  _test("do-while ASIs", "do foo\nwhile(0)",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_NUMBER,  
        TOKEN_CLOSE,   
  );

  _test("do-while stack", "do;while()bar",
        TOKEN_KEYWORD,   
        TOKEN_SEMICOLON, 
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_CLOSE,     
        TOKEN_SYMBOL,    
  );

  _test("do-while value-like", "do;while()\n/foo/",
        TOKEN_KEYWORD,   
        TOKEN_SEMICOLON, 
        TOKEN_KEYWORD,   
        TOKEN_PAREN,     
        TOKEN_CLOSE,     
        TOKEN_REGEXP,    
  );

  _test("strict", "'use strict'; let",
        TOKEN_STRING,    
        TOKEN_SEMICOLON, 
        TOKEN_KEYWORD,   
  );

  _test("arrow ASI bug", "{_ => {}}",
        TOKEN_BLOCK,  
        TOKEN_SYMBOL, 
        TOKEN_OP,     
        TOKEN_BLOCK,  
        TOKEN_CLOSE,  
        TOKEN_CLOSE,  
  );

  _test("arrow value bug", "{_ => {}/123/g;}",
        TOKEN_BLOCK,     
        TOKEN_SYMBOL,    
        TOKEN_OP,        
        TOKEN_BLOCK,     
        TOKEN_CLOSE,     
        TOKEN_REGEXP,    
        TOKEN_SEMICOLON, 
        TOKEN_CLOSE,     
  );

  _test("ASI in case", "switch { default: }",
        TOKEN_KEYWORD, 
        TOKEN_BLOCK,   
        TOKEN_KEYWORD, 
        TOKEN_COLON,   
        TOKEN_CLOSE,   
  );

  _test("dict method after colon", "void {a:a,get x() {}}",
        TOKEN_OP,      
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_COLON,   
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("dict closed on right", "+{x:1}",
        TOKEN_OP,     
        TOKEN_BRACE,  
        TOKEN_LIT,    
        TOKEN_COLON,  
        TOKEN_NUMBER, 
        TOKEN_CLOSE,  
  );

  _test("dict method", "void {[] () {}}",
        TOKEN_OP,    
        TOKEN_BRACE, 
        TOKEN_ARRAY, 
        TOKEN_CLOSE, 
        TOKEN_PAREN, 
        TOKEN_CLOSE, 
        TOKEN_BLOCK, 
        TOKEN_CLOSE, 
        TOKEN_CLOSE, 
  );

  _test("check import", "import foo, {zing as what} from 'blah' /foo/",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
        TOKEN_KEYWORD, 
        TOKEN_STRING,  
        TOKEN_REGEXP,  
  );

  _test("static", "class X { static x() {} }",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_BRACE,   
        TOKEN_KEYWORD, 
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("class prop is not symbol", "void class { x }",
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_CLOSE,   
  );

  _test("async assumed with dot", "x.async()",
        TOKEN_SYMBOL, 
        TOKEN_OP,     
        TOKEN_LIT,    
        TOKEN_PAREN,  
        TOKEN_CLOSE,  
  );

  
  _test("using await as name inside async", "async () => class await {}",
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_KEYWORD, 
        TOKEN_LIT,     
        TOKEN_BRACE,   
        TOKEN_CLOSE,   
  );

  _test("escape string", "'foo\\'bar'",
        TOKEN_STRING, 
  );

  _test("doesn't consume label", "{break}",
        TOKEN_BLOCK,   
        TOKEN_KEYWORD, 
        TOKEN_CLOSE,   
  );

  _test("host function stops statement", "abc\nfunction foo() {}",
        TOKEN_SYMBOL,  
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
  );

  _test("async part of function", "async\nfunction\nfoo() {}",
        TOKEN_KEYWORD, 
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
  );

  _test("await should treat ~ as unary op", "await ~123",
        TOKEN_OP,     
        TOKEN_OP,     
        TOKEN_NUMBER, 
  );

  _test("await should treat + as regular op", "await +123",
        TOKEN_OP,     
        TOKEN_OP,     
        TOKEN_NUMBER, 
  );

  _test("hashbang as valid comment", "#!hello", );

  _test("hashbang as comment", "#!hello /*\nfoo",
        TOKEN_SYMBOL, 
  );

  _test("hashbang with following comment", "#!hello\n

  _test("class extends op-like", "class X extends foo.bar { if() {} }",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_OP,      
        TOKEN_LIT,     
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("ternary has no value", "?:/foo/",
        TOKEN_TERNARY, 
        TOKEN_CLOSE,   
        TOKEN_REGEXP,  
  );

  _test("control with trailing statement", "if foo\nbar",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_SYMBOL,  
  );

  _test("attach statements", "if()try{}finally{}",
        TOKEN_KEYWORD, 
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_KEYWORD, 
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_KEYWORD, 
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
  );

  _test("solo async", "async(a)",
        TOKEN_SYMBOL, 
        TOKEN_PAREN,  
        TOKEN_SYMBOL, 
        TOKEN_CLOSE,  
  );

  _test("label inside block", "if foo: 1",
        TOKEN_KEYWORD, 
        TOKEN_LABEL,   
        TOKEN_COLON,   
        TOKEN_NUMBER,  
  );

  _test("return dict", "return {foo: foo}",
        TOKEN_KEYWORD, 
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_COLON,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
  );

  _test("return dict", "return {foo}",
        TOKEN_KEYWORD, 
        TOKEN_BRACE,   
        TOKEN_SYMBOL,  
        TOKEN_CLOSE,   
  );

  _test("regexp as start of block", "{/f/}",
        TOKEN_BLOCK,  
        TOKEN_REGEXP, 
        TOKEN_CLOSE,  
  );

  _test("orphaned keyword", "enum foo",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
  );

  _test("class var should close", "class X { #foo = 1\n#bar() {} }",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_OP,      
        TOKEN_NUMBER,  
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("class var should close", "class X { foo = () => {\n}\nbar() {} }",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_OP,      
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  _test("optional func", "func?.()",
        TOKEN_SYMBOL, 
        TOKEN_OP,     
        TOKEN_PAREN,  
        TOKEN_CLOSE,  
  );

  _test("class with *", "class Foo { x=()=>{}\n*bar() {} }",
        TOKEN_KEYWORD, 
        TOKEN_SYMBOL,  
        TOKEN_BRACE,   
        TOKEN_LIT,     
        TOKEN_OP,      
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_OP,      
        TOKEN_LIT,     
        TOKEN_PAREN,   
        TOKEN_CLOSE,   
        TOKEN_BLOCK,   
        TOKEN_CLOSE,   
        TOKEN_CLOSE,   
  );

  
  render_output = 1;
  testdef *p = &fail;
  printf("passed (%d/%d)\n", count - ecount, count);
  if (ecount)
  {
    printf("errors (%d/%d):\n", ecount, count);
  }
  while ((p = (testdef *)p->next))
  {
    printf("\n");
    run_testdef(p);
  }
  printf("\n");
  return err;
}
