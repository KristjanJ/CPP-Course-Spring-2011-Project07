#include <iostream>

#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"

using namespace boost;

#define PHILOSOPHER_COUNT 50

/*!
\brief Fork class.
*/
class Fork 
{
public:
	/*!
	\details Eats with a fork.
	*/
	void eat(void) 
	{
		if (this == 0) 
		{
			std::cout << "The fork is broken!" << std::endl;
			exit (0);
		}
		else
		{
			std::cout << "Munch...munch..." << std::endl;
		}
	}
};

Fork* forks[PHILOSOPHER_COUNT];
timed_mutex forkMutexes[PHILOSOPHER_COUNT];

Fork* takeFork(unsigned short seat) 
{
	Fork* fork = 0;

	if (forks[seat] != 0)
	{
		fork = forks[seat];

		// !!! If program should exit with an error, but does not, uncomment following line
		boost::this_thread::sleep(boost::posix_time::milliseconds(rand() % 50));

		forks[seat] = 0;
	}

	return fork;
}

void putForkDown(Fork* fork, unsigned short seat) 
{
	if (forks[seat] != 0) 
	{
		std::cout << "Fork " << seat << " has a clone!" << std::endl;
		exit (0);
	}

	forks[seat] = fork;
}

/*!
\brief Philosopher class.
*/
class Philosopher 
{
public:

	/*!
	\details Constructor.
	\param seat Seat.
	*/
	Philosopher (unsigned short seat) : m_seat (seat) 
	{ 

	}
	
	/*!
	\details Overloaded () operator.
	*/
	void operator () (void) 
	{
		while (true) 
		{
			bool lock1 = forkMutexes[(m_seat) % PHILOSOPHER_COUNT].timed_lock(boost::posix_time::milliseconds(10));
			
			if (lock1)
			{
				bool lock2 = forkMutexes[(m_seat + 1) % PHILOSOPHER_COUNT].timed_lock(boost::posix_time::milliseconds(10));

				if (lock2)
				{
					Fork* left = takeFork((m_seat) % PHILOSOPHER_COUNT);
					Fork* right = takeFork((m_seat + 1) % PHILOSOPHER_COUNT);

					if (left != 0 && right != 0)
					{
						left->eat();
						right->eat();
					}

					if (left != 0) putForkDown (left, (m_seat) % PHILOSOPHER_COUNT);
					if (right != 0) putForkDown (right, (m_seat + 1) % PHILOSOPHER_COUNT);

					forkMutexes[(m_seat) % PHILOSOPHER_COUNT].unlock();
					forkMutexes[(m_seat + 1) % PHILOSOPHER_COUNT].unlock();
				}
				else
				{
					forkMutexes[(m_seat) % PHILOSOPHER_COUNT].unlock();
				}
			}
		}		
	}
private:	
	unsigned short m_seat;
};


int main (int argc, char* argv[]) 
{
	srand (time (0));
	thread* threads[PHILOSOPHER_COUNT];

	//It makes sense to create fork objects before any threads try to use them.
	//Even though takeFork makes sure the fork is not a nullpointer, it is not good enough
	//because uninitialized variables contain random values (might not be 0).
	for (unsigned short i = 0; i < PHILOSOPHER_COUNT; i++) 
	{
		forks[i] = new Fork ();
	}

	for (unsigned short i = 0; i < PHILOSOPHER_COUNT; i++) 
	{
		threads[i] = new thread (Philosopher(i));
	}
		
	while (true);
}
