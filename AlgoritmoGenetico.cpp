// C++ implementation of the above approach 
#include <bits/stdc++.h> 
#include <limits.h> 
using namespace std; 

#define V 10            // Number of cities in TSP 

#define GENES ABCDEFGHIJ   // Names of the cities 

#define START 0       // Starting Node Value    

#define POP_SIZE 10  // Initial population size

// Structure of a GNOME 
// string defines the path traversed by the salesman while the fitness value 
// of the path is stored in an integer 

struct individual { 
	string gnome; 
	float fitness; 
}; 

// Function to return a random number from start and end 
int rand_num(int start, int end) { 
	int r = end - start; 
	int rnum = start + rand() % r; 
	return rnum; 
} 

// Function to check if the character has already occurred in the string 
bool repeat(string s, char ch) { 
	for (int i = 0; i < s.size(); i++) { 
		if (s[i] == ch) 
			return true; 
	} 
	return false; 
} 

// Function to return a mutated GNOME 
// Mutated GNOME is a string with a random interchange of two genes to create
// variation in species 
string mutatedGene(string gnome) { 
	while (true) { 
		int r = rand_num(1, V); 
		int r1 = rand_num(1, V); 
		if (r1 != r) { 
			char temp = gnome[r]; 
			gnome[r] = gnome[r1]; 
			gnome[r1] = temp; 
			break; 
		} 
	} 
	return gnome; 
} 

// Function to return a valid GNOME string required to create the population 
string create_gnome() { 
	string gnome = "0"; 
	while (true) { 
		if (gnome.size() == V) { 
			gnome += gnome[0]; 
			break; 
		} 
		int temp = rand_num(1, V); 
		if (!repeat(gnome, (char)(temp + 48))) 
			gnome += (char)(temp + 48); 
	} 
	return gnome; 
} 

// Function to return the fitness value of a gnome. 
// The fitness value is the path length of the path represented by the GNOME. 
float cal_fitness(string gnome) { 
	
    float map[V][V] = { { 0, 28.6, 10.725, 21.05, 60.45,39.65, 24.05, 34.125, 26.65, 26.975}, 
  		      			{ 28.06, 0, 25.35, 59.48, 61.43,46.475, 5.3625, 42.25, 3.7, 15.6  }, 
						{ 10.725, 25.35, 0, 42.25, 58.5,38.675, 23.4, 35.425, 26.975, 17.875 }, 
						{ 21.0, 59.48, 42.25, 0, 67.275,54.275, 57.85, 50.7, 61.1, 48.425 }, 
						{ 60.45, 61.43, 58.5, 67.275, 0,17.225, 63.05, 22.1, 34.35, 48.75},
						{ 39.65, 46.475, 38.675, 54.275, 17.225,0,49.725, 10.725, 50.375, 35.1}, 
  		      			{ 24.05, 5.3625, 23.4, 57.85, 63.05,49.725, 0, 43.875, 3.25, 18.525 }, 
						{ 34.125, 42.25, 35.425, 50.7, 22.1,10.725, 43.875, 0, 44.525, 29.25}, 
						{ 26.65, 3.7, 26.975, 61.1, 54.35,50.375, 3.25, 44.525,0, 19.5 }, 
						{ 26.975, 15.6, 17.875, 48.425, 48.75,35.1, 18.525, 29.25, 19.5,0  }
						 };


/*
0 Londres 458, Cercado de Lima 15033			central
1plaza de armas 				
2Circuito magico de las aguas				
3-museo del sitio Huaca Pucllana				
4-Parque de las leyendas				
5-Museo Larco				
6-Museo de la Inquisición del congreso				
7-Museo Nacional de Arqueología, Antropología e Historia del Perú 				
8-Catacumbas del Convento de San Francisco				
9-Casa Matusita				
	*/

	float f = 0; 
	for (int i = 0; i < gnome.size() - 1; i++) { 
		if (map[gnome[i] - 48][gnome[i + 1] - 48] == 10) 
			return 10; 
		f += map[gnome[i] - 48][gnome[i + 1] - 48]; 
	} 
	return f; 
} 

// Function to return the updated value of the cooling element. 
int cooldown(int temp) { 
	return (90 * temp) / 100; 
} 

// Comparator for GNOME struct. 
bool lessthan(struct individual t1, 
			struct individual t2) { 
	return t1.fitness < t2.fitness; 
} 

// Utility function for TSP problem. 
void TSPUtil(float map[V][V]) { 
	int gen = 1;       // Generation Number
	int gen_thres = 10; // Number of Gene Iterations

	vector<struct individual> population; 
	struct individual temp; 

	// Populating the GNOME pool. 
	for (int i = 0; i < POP_SIZE; i++) { 
		temp.gnome = create_gnome(); 
		temp.fitness = cal_fitness(temp.gnome); 
		population.push_back(temp); 
	} 

	cout << "\nInitial population: " << endl 
		<< "GNOME	    FITNESS VALUE\n"; 
	for (int i = 0; i < POP_SIZE; i++) 
		cout << population[i].gnome << "       "
			<< population[i].fitness << endl; 
	cout << "\n"; 

	bool found = false; 
	int temperature = 10000; 

	// Iteration to perform population crossing and gene mutation. 
	while (temperature > 1000 && gen <= gen_thres) { 
		sort(population.begin(), population.end(), lessthan); 
		cout << "\nCurrent temp: " << temperature << "\n"; 
		vector<struct individual> new_population; 

		for (int i = 0; i < POP_SIZE; i++) { 
			struct individual p1 = population[i]; 

			while (true) { 
				string new_g = mutatedGene(p1.gnome); 
				struct individual new_gnome; 
				new_gnome.gnome = new_g; 
				new_gnome.fitness = cal_fitness(new_gnome.gnome); 

				if (new_gnome.fitness <= population[i].fitness) { 
					new_population.push_back(new_gnome); 
					break; 
				} 
				else { 
					// Accepting the rejected children at 
					// a possible probablity above threshold. 
					float prob = pow(2.7, 
								-1 * ((float)(new_gnome.fitness 
										- population[i].fitness) 
										/ temperature)); 
					if (prob > 0.5) { 
						new_population.push_back(new_gnome); 
						break; 
					} 
				} 
			} 
		} 

		temperature = cooldown(temperature); 
		population = new_population; 
		cout << "Generation " << gen << " \n"; 
		cout << "GNOME      FITNESS VALUE\n"; 

		for (int i = 0; i < POP_SIZE; i++) 
			cout << population[i].gnome << "        "
				<< population[i].fitness << endl; 
		gen++; 
	} 
} 

int main() { 
    float map[V][V] = { { 0, 28.6, 10.725, 21.05, 60.45,39.65, 24.05, 34.125, 26.65, 26.975}, 
  		      			{ 28.06, 0, 25.35, 59.48, 61.43,46.475, 5.3625, 42.25, 3.7, 15.6  }, 
						{ 10.725, 25.35, 0, 42.25, 58.5,38.675, 23.4, 35.425, 26.975, 17.875 }, 
						{ 21.0, 59.48, 42.25, 0, 67.275,54.275, 57.85, 50.7, 61.1, 48.425 }, 
						{ 60.45, 61.43, 58.5, 67.275, 0,17.225, 63.05, 22.1, 34.35, 48.75},
						{ 39.65, 46.475, 38.675, 54.275, 17.225,0,49.725, 10.725, 50.375, 35.1}, 
  		      			{ 24.05, 5.3625, 23.4, 57.85, 63.05,49.725, 0, 43.875, 3.25, 18.525 }, 
						{ 34.125, 42.25, 35.425, 50.7, 22.1,10.725, 43.875, 0, 44.525, 29.25}, 
						{ 26.65, 3.7, 26.975, 61.1, 54.35,50.375, 3.25, 44.525,0, 19.5 }, 
						{ 26.975, 15.6, 17.875, 48.425, 48.75,35.1, 18.525, 29.25, 19.5,0  }
						 };
		
					
			
    TSPUtil(map); 
}