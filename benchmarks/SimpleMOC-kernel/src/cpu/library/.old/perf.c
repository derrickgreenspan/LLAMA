int _get_performance_ratio(void)
{
	FILE *ofp = fopen(MEM_INFO_LOCATION, "r");
	if(!ofp)
	{
		/* This error handling should go into a different function */
		fprintf(stderr, "Error opening %s: %s.\n",
			       	MEM_INFO_LOCATION, strerror(errno));
		fprintf(stderr, "This error usually occurs if you have "
				"not run the included benchmark utility yet.\n"
			       	"Alternatively, check if you have read access to %s.\n", 
				MEM_INFO_LOCATION);
		fprintf(stderr, "Program will now exit.\n");
		exit(-1);
	}
	fscanf(ofp, "%d", &performance_ratio);
	fclose(ofp);
	if(performance_ratio < 0)
	{
		fprintf(stderr, "WARNING: The performance ratio found in %s was less than 0, "
				"which is not sane. Program will continue but will likely "
				"crash or do unusual things.\n", MEM_INFO_LOCATION);
		fprintf(stderr, "It is recommended to run the benchmark program again.\n");
	}
	return performance_ratio;
}
