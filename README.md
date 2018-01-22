# DYAF
Do Yourself A Favor is a simple extensible parser and AST generator (and in future compiler).

By default the parser is unable to parse anything and the AST will only contain a block with all the code but 
via the ```Extension``` class is possible to define new symbols and the extra parsing capabilities.

A simple example is ```scidown.h``` extension.

## Example 

Using the ```scidown``` extension on the text:

```  
# Title
Another _line_...
## A sub-chapter
etc *etc* etc...
# A Second chapter
this is ~not~ a [link](url).
 Bla -bla- bla...
This @(ref1) is a reference to something not yet referenced.

```

DYAF generate the following **AST**

```
┬─ NaN()
├┬─ Chapter(Title)
│├┬─ Line()
││├── NaN(): [Another ]
││├┬─ underline()
│││└── NaN(): [line]
││└── NaN(): [...]
│└┬─ SubChapter( A sub-chapter)
│ └┬─ Line()
│  ├── NaN(): [etc ]
│  ├┬─ bold()
│  │└── NaN(): [etc]
│  └── NaN(): [ etc...]
└┬─ Chapter(A Second chapter)
 ├┬─ Line()
 │├── NaN(): [this is ]
 │├┬─ deleted()
 ││└── NaN(): [not]
 │├── NaN(): [ a ]
 │├── link(link, url): []
 │└── NaN(): [.]
 ├┬─ Line()
 │├── NaN(): [ Bla ]
 │├┬─ italic()
 ││└── NaN(): [bla]
 │└── NaN(): [ bla...]
 └┬─ Line()
  ├── NaN(): [This ]
  ├── reference(ref1): []
  └── NaN(): [ is a reference to something not yet referenced.]
```
