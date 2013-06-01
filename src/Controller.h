#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <map>
#include <string>
#include <vector>


typedef std::pair<int,int> procParamP_t; //first: jobID to return when the job is finished; second: Duration in seconds
typedef std::vector<int, procParamP_t> procsToStartV_t; //first: Index of Processor; Second Params to start the job
typedef std::vector<std::string> processorV_t; //List of Processor Type Names to start
class ServiceReader;

class Controller {
public:
	/**
	 * Reads the ini files, and might end the program if there's an error
	 *
	 * @Param filename Name including path of the System.ini
	 */
	Controller(std::string filename);


	/**
	 * Reads the ini files from a standard location, and might end the program if there's an error
	 *
	 * @Param filename Name including path of the System.ini
	 */
	Controller();
    
    ~Controller();

	/**
	 * Returns an list of the type of Processors needed. For each Listmember there needs to be
	 * started an according Taskprocessor in Rose/RT.
	 * The Index here needs to be remembered because the TaskProcessers will be Accessed by this index.
	 *
	 * @Param ProcessorList OUT List of names of processors to start, list is not sorted by type
	 * @Return Number of members of this list; -1 if an error occurred
	 */
	int getServiceProcessors(processorV_t ** processorList);

	/**
	 * Returns a List of procs with params to start
	 * @Param procsToStart OUT: List of procs to start; Undefined if we need to wait or all Jobs are done
	 * @Return: Number of Procs to start; -1 If all Jobs are finished; 0 if we need to wait for jobs to finish
	 */
	int getNextJob(procsToStartV_t ** procsToStart);

	/**
	 * Tells the Controller that a single Taskprocessor has finished the job with the given ID
	 * @Param id ID of the job that has finished
	 * @Return true if everything's allright; False if an error occurred
	 */
	int jobFinished(int jobID);

	/**
	 * To Call if something is went wrong on while trying to start the job with the given ID
	 * shouldn't happen so I don't know jet how to implement this or what it will return
	 * maybe we terminate everything then
	 */
	int jobStartFailure(int jobID);

	/**
	 * To Call if an Processor throws an exception (takes longer than expected). I don't think we should react to this but it was said to implement
	 */
	int jobException(int jobID);

	/**
	 * To Call if an processor Terminates Unexpected
	 * Probably should Terminate everything, not sure about this.
	 */
	int serviceProcessorUnexpectedTerminated(int processorID);
    
private:
    ServiceReader * serviceReader; /// ServiceReader of the System.ini
};

#endif /* CONTROLLER_H */
