# DYAF
Do Yourself A Favor is a simple extensible parser and AST generator (and in future compiler).

By default the parser is unable to parse anything and the AST will only contain a block with all the code but 
via the ```Extension``` class is possible to define new symbols and the extra parsing capabilities.

A simple example is ```scidown.h``` extension.