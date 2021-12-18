# CS3203 Team 21

## Building the project

**Important**: the following instructions assume that you are using a MacOS environment with `cmake` and `llvm` already installed via `brew`, and that you are currently in the root directory of the project (ie. where the `Team21` directory is located).

Create a new `/build` directory within the root directory and change into it:

```sh
mkdir build
cd build
```

Then, generate the production files using:

```sh
# while in /build
cmake ../Team21/Code21/ -DCMAKE_BUILD_TYPE=Release
```

And build these files for release using:

```sh
# while in /build
cmake --build . --config Release
```

Run the unit and integration tests by using:

```sh
# while in /build
# run all unit tests:
src/unit_testing/unit_testing
# run all integration tests:
src/integration_testing/integration_testing
```

Lastly, change into the AutoTester directory (`~/build/src/autotester`), and verify that the AutoTester can run by using:

```sh
# while in /build
# change into AutoTester dir
cd src/autotester
# run the autotester with a supplied test case:
./autotester ../../../Team21/Tests21/prev_iterations/2_medium_source.txt ../../../Team21/Tests21/prev_iterations/2_medium_queries.txt ../../../out.xml
```

## Using AutoTester for previous iterations (1/2)

Test suites for the previous iterations are defined within the `~/Team21/Tests21/prev_iterations` directory. The first number in the filename represent which test suite they are. For example, the source file `1_medium_source.txt` is to be tested against the query file `1_medium_queries.txt`. A human readable and line annotated source program is also provided, with `_readable` in the file name (however, this source file should not be used for the AutoTester).

When encountering invalid SIMPLE source programs, the AutoTester will immediately exit with error code 1 without evaluating any queries nor generating the `out.xml` file. The file `1_invalid_source.txt` is used to test for this behaviour (since the queries will not be evaluated, any query file can be used together with this invalid source).

### Running against the testcases

To run the AutoTester, first make sure that you have already successfully [built the project](#building-the-project). Next, change directory to where the `AutoTester` executable is located (ie. `~/build/src/autotester`):

```sh
# eg. run the second test suite:
./autotester ../../../Team21/Tests21/prev_iterations/2_medium_source.txt ../../../Team21/Tests21/prev_iterations/2_medium_queries.txt ../../../out.xml
# eg. run the third test suite:
./autotester ../../../Team21/Tests21/prev_iterations/3_hard_source.txt ../../../Team21/Tests21/prev_iterations/3_hard_queries.txt ../../../out.xml
```

This will generate the `out.xml` output file in the root directory.

### Writing custom testcases

> Refer to [the wiki](https://github.com/nus-cs3203/project-wiki/wiki/Autotester-Integration-and-Testing#format-of-simple-queries) for the required format of the query files

To accommodate the [execution of the `run-checks.py` file](#using-the-run-checkspy-helper-file), make sure that the source and query files have the following naming convention:

- A pair of source and query file must **start** with the same unique number (ie. `34_source.txt` and `34_queries.txt`)
- Valid source files must **end** with the word `source` (ie. `1_YOUR_DESCRIPTION_HERE_source.txt`)
- Valid query files must **end** with the word `queries` (ie. `1_YOUR_DESCRIPTION_HERE_queries.txt`)
- Annotated source files must have the word `readable` within the name (ie. `1_source_readable.txt`)
  - Any filenames with `readable` will be filtered out
- Invalid source files must have the word `invalid` within the name (ie. `1_invalid_source.txt`)
  - Any filenames with `invalid` will be assumed to be an invalid source

## Using AutoTester for iteration 3

The extensions for NextBip/NextBip* and AffectsBip/AffectsBip* can be toggled on or off when running the AutoTester by setting the `EXTENSION` environment variable:

- Default (ie. environment variable not set): runs SPA without any extensions
- `NB`: runs SPA with only the NextBip and NextBip* extensions
- `AB`: runs SPA with only the AffectsBip and AffectsBip* extensions
- `NBAB` or `ABNB`: runs SPA with all NextBip/NextBip* and AffectsBip/AffectsBip* extensions

```sh
# Run without any extensions enabled:
./autotester ../../../Team21/Tests21/3_relationship_emphasis_source.txt ../../../Team21/Tests21/3_relationship_emphasis_queries.txt ../../../out.xml
# Run with only the NextBip and NextBip* extensions:
EXTENSION=NB ./autotester ../../../Team21/Tests21/3_relationship_emphasis_source.txt ../../../Team21/Tests21/3_relationship_emphasis_queries.txt ../../../out.xml
# Run with only the AffectsBip and AffectsBip* extensions:
EXTENSION=AB ./autotester ../../../Team21/Tests21/3_relationship_emphasis_source.txt ../../../Team21/Tests21/3_relationship_emphasis_queries.txt ../../../out.xml
# Run with all extensions enabled:
EXTENSION=NBAB ./autotester ../../../Team21/Tests21/3_relationship_emphasis_source.txt ../../../Team21/Tests21/3_relationship_emphasis_queries.txt ../../../out.xml
```

## Using the `run-checks.py` helper file

To run the `run-checks.py` **using python 3**:

```sh
# To run without NBAB extensions:
python3 run-checks.py
# To run with NBAB extensions:
EXTENSION=NBAB python3 run-checks.py
```

It will do the following in order:

1. Run all unit tests
2. Run all integration tests
3. Run all autotester test cases in `Test21` directory

The execution should exit with error code 0 (ie. no errors) if all tests passed, and exit with error code 1 (ie. errors exist) if *any* tests fail. The execution *will not terminate early* if any testcases fail (unless it is a system error).

## Using the `generate-source.py` random SIMPLE source generator

To immediately start generating a random source with default values, run:

```sh
python3 generate-source.py
```

- Use the `-h` flag to see all values that can be tweaked (ie. `python3 generate-source.py -h`)
- 3 files will be written to `~/__generated_source`)
  - `source.txt`: to be parsed by the parser
  - `source_readable.txt`: contain line number comments
  - `metadata.txt`: contains metadata about the program formed

## Toggling query optimizations

Query Optimizations can be toggled manually at the top of the `QueryEvaluator.cpp` file.
Recompilation is required after changing the flags.   
Note that the last flag is set to FALSE by default for our SPA and should remain so for all testing purposes.

```c++
/* Query Optimizer Flags
 * Note: If the optimize_query flag is set to false when calling EvaluateQuery, all optimizations will be disabled. 
 */

const bool REMOVE_DUPLICATE_CLAUSE = true;    // Removes duplicated clauses of the same type, same design abstraction and same params.
const bool SORT_CLAUSES = true;               // Sorts clauses based on number of synonyms, followed by type of clause (With Clause < Such That Clause < Pattern Clause)
const bool GROUP_BEFORE_MERGE = true;         // Groups Result Tables based on overlapping synonyms. Clauses with no related synonyms will not be merged.
const bool SORT_TABLES_BEFORE_BFS = true;     // Sorts result tables based on table size before BFS. BFS will start from the smallest table.

// default: false for this optimisation
const bool SORT_TABLES_BEFORE_MERGE = false;  // During BFS, sort each node's neighbours based on table size. Merging will start from the smallest table.
```
