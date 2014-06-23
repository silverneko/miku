miku
====

```bash
$ git clone https://github.com/silverneko/miku.git
```

####How to start the judge client
```bash
$ make
$ sudo ./start_script
```
some configuration can be made inside startscript like:
-v(, or --verbose) for extra verbosity
-p(, or --parallel) [NUMBER] to have maxium [NUMBER] of parallel 
processes running when judging. However this may cause verbosity 
messages unreadable.
