# CP2-IO
Assignment for C2-THE-C2 Course

# Homework based on Day 1 exercises
If we take a look at the code from Day 1, https://github.com/jahreda/c2-the-p2/tree/main/IO/day1, we have an opportunity to ﬁll branches with multiple types of data at once. Running N = 1e8 and larger will take at least an hour and it will blow up the ROOT ﬁles of each ﬂush conﬁguration to ~45 GB. Keep this at or under 1e7, preferably 1e6!!

1. How might mixing the baskets with different kinds of data affect compression of the data upon ﬁlling the tree and saving to ﬁle? Does this have an effect with the overall disk usage or CPU walltime?

**Answer** 
`IO/day1/Screenshots` contains two folders, `All_Floats` and `Mixed_Data_Types`. `All_Floats` contains screenshots of the five autoflush settings when the files were writted with the same datatypes. This was done by changing `vint` variable in `writeROOTFile.cpp` to a float type. `Mixed_Data_Types` contains screenshots of the five autoflush settings with `vint` set to integer type. Each folder also contains a `data.txt` file, where I manually saved the data from provided in the screenshot - later used in `data_types_analysis.ipynb`. 

The following are the results from changing all variables, px, py, pz and vint to float types versus maintaining vint to integer type: 
![analysis](IO/day1/datatype_analysis.png)
