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
processes to evaluate usercode. However this may cause verbosity  
messages unreadable.  
-b [NUMBER] to set sandbox indexing offset. Need to be set to an  
appropriate number if running multiple judges on one computer.  
-a(, or --aggressive-update) add this to aggressivly update  
verdict and result.  

###Known issues
sandbox doesn't set output limit: https://github.com/cms-dev/cms/issues/309
