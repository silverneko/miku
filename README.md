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
some configuration can be made inside startscript like:<br>
-v(, or --verbose) for extra verbosity<br>
-p(, or --parallel) [NUMBER] to have maxium [NUMBER] of parallel <br>
processes to evaluate usercode. However this may cause verbosity <br>
messages unreadable.<br>
-b [NUMBER] to set sandbox indexing offset. Need to be set to an<br>
appropriate number if running multiple judgse on one computer.<br>
-a(, or --aggressive-update) add this to aggressivly update <br>
verdict and result.<br>

###Known issues
sandbox doesn't set output limit: https://github.com/cms-dev/cms/issues/309
