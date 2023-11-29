# earley-algorithm
Implementation of Earley's algorithm that enables to check if word belongs to context-free grammar.

**To build project, you should run following command:**

` mkdir build && cd build && cmake .. && make`

After that, you have several options.

* **Run tests**

You just type `./tests` and it will run the tests. Next to you, in build directory, you can find text files `log_test_*` that contain detailed description of algorithm's work.

* **Run program with your data**

Type `./earley-algorithm` and then enter your data in following format:
```
<number of nonterminals> <number of terminals> <number of rules>
<nonterminals>
<terminals>
<rules>
<starting nonterminal>
<number or requests>
<requests>
```
Each nonterminal should be a capital letter, each terminal should be a lowercase letter, each rule should look like `A->α`(no spaces is crucial!).

As an example, input below is correct:

```
1 2 2
S
ab
S->
S->aSbS
S
2
aabb
ababaaabbb
```

You can also see detailed logs in `log`.
