# Grip Strength Force Calculator

Updated 01/05/19

This program is intended for [NIST Grasping and Manipulation Performance and Benchmarking tests](https://www.nist.gov/el/intelligent-systems-division-73500/robotic-grasping-and-manipulation-assembly) using a 2-load cell split cylinder artifact.  It takes a 3-column .csv file and parses repetition data to calculate the mean and standard deviation of each repetition as well as the trial as a whole.

### Use

The third column of the .csv should represent the combined load cell force reading in N.  By default gsfc assumes > 60 non-zero elements per repetition to account for any noise.  (A repetition with a sampling rate of 25 Hz over 5 seconds should contain about 125 non-zero elements.) This can be adjusted by changing the REPELEMS value in gsfc.cpp.

Assuming some amount of error at the start and finish of each grasp cycle, this program trims a percentage of the total number of elements in a repetition from the beginning and end, e.g. 20% trims 10% from the beginning and 10% from the end. The default for this is 20 but can be changed with an optional command line argument.

Likewise accounting for outlying repetitions in the final trial mean, gsfc sorts the repetition means and trims the top and bottom by the same trim percent.


### Prerequisites

* Uses c++11
* Compile with g++

### Installing

In a bash terminal inside the directory:

```
make
```

## Running the program

After building the .exe, put any .csv files in the csvs directory
In a bash terminal inside the parent directory:

```
./gsfc csvs/[name_of_file]
```

For multiple .csvs:

```
for i in csvs/*; do ./gsfc $i; done
```

Optional integer argument for trim percent (20 default):

```
./gsfc csvs/[name of file] [percent]
```

Output files will appear in the data folder. 


## Authors

* **Dan Hemphill** - *daniel_hemphill@student.uml.edu*

