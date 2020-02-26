## README
This software is to be used on a web server with PHP >=7 (if you are running Apache locally, I suggest you increase your `LimitRequestLine` and `LimitRequestFieldSize` in your `apache2.conf` or similar to handle more interesting examples, since, at the moment, the regexp is sent with a GET).

It's an exercise for the course on [Languages, Compilers, Interpreters](https://github.com/ranma42/languages-compilers-interpreters-2019) offered at the University of Pisa.

The intended folder structure is the following (you can adjust it in [commons.php](./web/commons.php) and Makefile).

For serious use, rewrite the software ^^ At least run the regex parser on top of a server distributing loads (easier: if you have php 7.4, with `proc_open` you can avoid opening a shell to invoke the parser in [regexp.php](./web/regexp.php) :), add a cache, error handling etc...

![Tree Dirs](http://geimaxpower.altervista.org/Schermata_da_2020-02-17_23-56-26.png)   
![Tree](http://geimaxpower.altervista.org/Schermata_da_2020-02-17_23-57-56.png)  