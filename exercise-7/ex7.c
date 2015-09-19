#include <stdio.h>

int main(int argc, char *argv[])
{
    int bugs = 100;
    double bug_rate = 1.2;
    
    printf("You have %d bugs, rate is %011.3f\n", bugs, bug_rate);
    
    unsigned long universe_of_defects = 1L * 1024L * 1024L * 1024L * 1024L * 1024L;
    printf("The entire universe has %ld bugs\n", universe_of_defects);
    
    double expected_bugs = bugs * bug_rate;
    printf("You are expected to have %f bugs\n", expected_bugs);
    
    double part_of_universe = expected_bugs / universe_of_defects;
    printf("Part of universe %e\n", part_of_universe);
    
    char nul_byte = '\0';
    double care_percentage = bugs * nul_byte;
    printf("You should care %.02f%%\n", care_percentage);
    
    return 0;
}