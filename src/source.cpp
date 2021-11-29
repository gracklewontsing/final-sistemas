// Author: Jesus Enrique Ruiz Aguilar
// Matricula: A00758756
// Sistemas Inteligentes
// Programa para calcular dietas diarias dependientes de un menu del dia

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>    
#include <time.h> 
#define POPULATION_SIZE 10
#define POPULATION_TYPE 25     

using namespace std;
enum activityLevel {BA, LA, MA, VA, XA};
enum objective {FIT, MUSCLE, OPT, ANY};
enum Gender {MALE, FEMALE}; 
int population[POPULATION_SIZE][POPULATION_TYPE];
int new_generation[POPULATION_SIZE][POPULATION_TYPE];  
int child[POPULATION_TYPE];
double aptitude[POPULATION_SIZE];
int parent_a[POPULATION_TYPE];
int parent_b[POPULATION_TYPE];

class FoodItem {
  //calories are obtained  as follows
  // carbs: 4 calories per gram, eg : carb = 10g, calories = 40
  // fats: 9 cals per gram
  // protein: 4 cals per gram
  public:
    string name;
    int carbs;
    int fats;
    int proteins;
    int calories;
    FoodItem(string n, int c, int f, int p){
      name = n;
      carbs = c;
      fats = f;
      proteins = p;
      calories = (proteins * 4) + (fats * 9) + (carbs * 4);
    }
};

//thisis all just a calorie limit check
class Human {
  public: 
    string name;
    Gender gender;
    int height; //cm
    int weight; //kg
    int age; //years
    activityLevel level; //now as barely, lightly, moderately, very and extremely active  
    objective target;
    double dailyIntake;
    double tdee;
    double neededCarbs = 0;
    double neededFats = 0;
    double neededProteins = 0;
    Human(string n, Gender g, int h, int w, int a, activityLevel l, objective t){
      name = n;
      gender = g;
      height = h;
      weight = w;
      age = a;
      level = l;
      target = t;
        // daily calorie intake needs are as follows
        // Men: calories/day = 10 x weight (kg) + 6.25 x height (cm) – 5 x age (y) + 5
        // Women: calories/day = 10 x weight (kg) + 6.25 x height (cm) – 5 x age (y) – 161
      if(gender == MALE){
        dailyIntake = (10 * weight)+(6.25*height)-(5*age)+5;
      }
      else {
        dailyIntake = (10 * weight)+(6.25*height)-(5*age)-161;
      }
        //activity levels are as follows 
        // Sedentary: x 1.2 (limited exercise)
        // Lightly active: x 1.375 (light exercise less than three days per week)
        // Moderately active: x 1.55 (moderate exercise most days of the week)
        // Very active: x 1.725 (hard exercise every day)
        // Extra active: x 1.9 (strenuous exercise two or more times per day)
        //multiplying daily by act levels results in TDEE (total daily energy expenditure)
        //TDEE is a rough estimate of how many calories you need daily
        //To build muscle, one needs more calories with a preference on protein
        //To lose weight, one needs less calories with a decline on carbs
      switch (level) {
        case BA: tdee = dailyIntake * 1.2; break;
        case LA: tdee = dailyIntake * 1.375; break;
        case MA: tdee = dailyIntake * 1.55; break;
        case VA: tdee = dailyIntake * 1.725; break;
        case XA: tdee = dailyIntake * 1.9; break;
      }
        //example for macronutrient diet
        // macronutrient breakdown: (must be's)
        // Carbs: 45–65% of total calories ( variable depending on objective)
        // Fats: 20–35% of total calories (variable depending on objective)
        // Proteins: 10–35% of total calories (variable depending on objective)
        //consider 4 options : looking to lose weight, looking to attain muscle, optimal, any
        //looking to lose weight 
        //looking to attain muscle 
        //optimal 
        //any(no bias as long as the previous macro conditions are satisfied)
        //micronutrients (minerals, vitamins) are excluded as the needed intake will be allegedly consumed from most foods
        //although meals have also a bias per period, they will be excluded
      switch (target) { //set lower limits
        case FIT: neededFats = (tdee * .30)/9; neededProteins = (tdee * .35)/4; break; //as long as fats and proteins exceed these amounts, it is accepted
        case MUSCLE: neededProteins = (tdee* .40)/4; break; //as long as proteins equal or exceed this amount, it is accepted
        case OPT: neededCarbs = (tdee*.30)/4; neededFats = (tdee * .25)/9; neededProteins = (tdee*.35)/4; break; //as long as the lower limits are met or exceeded, it is accepted
        case ANY: neededCarbs = (tdee*.35)/4; neededFats = (tdee * .30)/9; neededProteins = (tdee*.35)/4; break;
      }

    }
};



// mutation in a genome
void mutate()
{    
    int r = rand() % POPULATION_TYPE;
    if(child[r] == 0){
      child[r] = 1;
    }
    else{
      child[r]=0;
    }        
}

//food items mostly taken from https://www.nutritionix.com/food
//genes for the individual, aka menu
vector<FoodItem> initMenu(){
   vector<FoodItem> menu = {
    FoodItem("Chicken Sandwich", 39, 21, 30),
    FoodItem("Lemnas Bread (2 slice)", 34,2,10),
    FoodItem("Gray Fruit",25,5,24),
    FoodItem("Salad(dressed)",12,24,8),
    FoodItem("Lum Berry",10,5,0),
    FoodItem("Apple",10,2,0),
    FoodItem("Normal Cheez",4,15,24),
    FoodItem("Pork Rinds",0,27,3),
    FoodItem("Boneless Chicken Wings",20,20,40),
    FoodItem("Snickers",35,14,4),
    FoodItem("Ramen",111,35,48),
    FoodItem("Pepperoni Pizza (slice)",35,21,13),
    FoodItem("Pepperoni Pizza (4 slices)",140,82,52),
    FoodItem("Burger, generic",40,27,34),
    FoodItem("Steaks", 0,54,94),
    FoodItem("Chicken Nuggets (10 pz)", 23, 29, 25),
    FoodItem("Fish Fillet", 0, 5,44),
    FoodItem("Salmon Fillet",0,20,50),
    FoodItem("Ham Sandwich", 48,14,22),
    FoodItem("Fried eggs",2,28,26),
    FoodItem("Bacon(4 slices)", 0,16,16),
    FoodItem("Fried rice(cup)", 45,4,6),
    FoodItem("Chicken Breasts", 0,11,71),
    FoodItem("Tacos al pastor(4)", 8,20,50),
    FoodItem("Cereal bowl", 42,4,7)
   };
  return menu;
}

void populate() {
  for (int i = 0; i < POPULATION_SIZE ; i++){
    for(int j = 0; j < POPULATION_TYPE; j++){
      population[i][j] = rand() % 2;
    }
  }
}
vector<FoodItem> menu = initMenu();
void calcNutrients(Human user){      
  int calorieCount=0;
  int carbCount=0;
  int fatCount=0;
  int proteinCount=0;    
  for(int i = 0; i<POPULATION_SIZE; i++){
    aptitude[i] = 0;
    for(int j = 0; j < POPULATION_TYPE;j++){    
      if(population[i][j] == 1){
        //cout<<menu[j].name<<endl;
        calorieCount += menu[j].calories;
        carbCount += menu[j].carbs;
        fatCount += menu[j].fats;
        proteinCount += menu[j].proteins;
      }
    }
    if(calorieCount != user.tdee){
      aptitude[i] += abs((user.tdee-calorieCount));        
    }
    if(abs(carbCount-user.neededCarbs)>15 && user.neededCarbs!=0){
      aptitude[i] += abs((user.neededCarbs-carbCount));
    }
    if(fatCount < user.neededFats && user.neededFats!=0){
      aptitude[i] += abs((user.neededFats-fatCount))*.7;
    }
    if(abs(proteinCount - user.neededProteins)>15  && user.neededProteins!=0){
      aptitude[i] += abs((user.neededProteins-proteinCount));      
    }
    calorieCount=0;
    carbCount=0;
    fatCount=0;
    proteinCount=0;
    //cout << aptitude[i]<<endl;
  }
}

//returns a child
void cross(){
  int random_a = rand()% POPULATION_TYPE;
  int random_b = rand()% POPULATION_TYPE;
  while(random_b == random_a){
    random_b = rand()% POPULATION_TYPE;
  }
  //start as parent b
  for(int i = 0; i<POPULATION_TYPE; i++){
    child[i] = parent_b[i];
  } 
  //internal range eg.: { 0, 1, 1,[0,1,0,1],0,1 }
  if(random_a < random_b){
    //cross genes from a to child
    for(int i = random_a; i<random_b;i++){
      child[i] = parent_a[i];
    }
  }
  //out of bound range eg.: { 0, 1, 1],0,1,0,1,[0,1 }
  else if(random_a > random_b) {
    //cross genes from a to child
    for(int i = random_a; i<POPULATION_TYPE;i++){
      child[i] = parent_a[i];
    }
    for(int i = 0; i<random_b;i++){
      child[i] = parent_a[i];
    }
  }  
}

void selection_cross_mutate(){
  int random_a = rand() % POPULATION_SIZE;  
  int random_b = rand() % POPULATION_SIZE;  
  while(random_b == random_a)
     random_b = rand() % POPULATION_SIZE;      
  
  double fitter=999999;
  int fitter_pos=INT8_MAX;
  double fittest=999999;
  int fittest_pos=INT8_MAX;
  //select 2 fittest
  for(int i = 0; i<POPULATION_SIZE; i++){    
    if(aptitude[i]<fittest){        
      fitter = fittest;
      fitter_pos = fittest_pos;
      fittest = aptitude[i];
      fittest_pos=i;                   
    }
    else if(aptitude[i]<fitter && aptitude[i]!= fittest){
      fitter=aptitude[i];
      fitter_pos=i;              
    }      
  }      

  //pass 2 fittest onwards
  for(int j=0; j<POPULATION_TYPE;j++){ 
    new_generation[0][j] = population[fittest_pos][j];
    new_generation[1][j] = population[fitter_pos][j];      
  }  
  //select 8 others randomly for reproduction
  for(int k = 2; k<POPULATION_SIZE;k++){
    //select parents randomly
    for(int i = 0; i<POPULATION_TYPE; i++){
      parent_a[i] = population[random_a][i];
      parent_b[i] = population[random_b][i];    
    }

    //cross parents to a child
    cross();  

    //mutate the childs genomes
    if(((rand()%10) +1) <= 2)
      mutate();           
      
    //imprint child into new generation
    for(int j=0; j<POPULATION_TYPE;j++) 
      new_generation[k][j] = child[j];
  }        
}

void overwrite(){
  for (int i=0;i<POPULATION_SIZE;i++){
    for (int j =0; j<POPULATION_TYPE;j++){
      population[i][j]=new_generation[i][j];
    }
  }
}

void genetic(Human user){
  populate();
  double fitness=999999;
  int generations = 0;
  calcNutrients(user);
  while(fitness > 25 && generations < 5000){           
    //select, cross, mutate to generate ten children
    selection_cross_mutate();
    //population = new_generation;
    overwrite();    
    generations++;
    //test
    calcNutrients(user);
    fitness = aptitude[0];
    for(int i = 0; i<POPULATION_SIZE;i++){
      if(aptitude[i]<fitness){
        fitness = aptitude[i];
      }
    }
    cout << "GENERATION "<<generations<< " -> FITNESS: "<< fitness<< endl;
  }  
  //print results
  int best;
  for (int i = 0; i<POPULATION_SIZE;i++){
    if(fitness == aptitude[i]){
      best = i;
    }
  }
  cout << "A proper diet for the day was found: " <<endl;
  int cals=0, carbs=0, fats=0, prots=0;
  for (int i = 0; i<POPULATION_TYPE;i++){    
    if(population[best][i] == 1){      
      cout << menu[i].name << " (" << menu[i].calories<< "cals, "<< menu[i].carbs<< "g, "<< menu[i].fats<< "g, "<< menu[i].proteins<< "g)."<<endl;      
      cals+= menu[i].calories;
      carbs += menu[i].carbs;
      fats += menu[i].fats;
      prots += menu[i].proteins;
    }    
  }
  cout << "Your totals are: " << endl;
  cout << cals << " total calories against "<< user.tdee<< " needed calories."<<endl;
  cout<< carbs << "g of total carbs against "<< user.neededCarbs<<"g needed carbs."<<endl;
  cout<< fats <<  "g of total fats against "<< user.neededFats<<"g needed fats."<<endl;
  cout<< prots <<  "g of total proteins against "<< user.neededProteins<<"g needed proteins."<<endl;
}


int main()
{      
  srand(time(0));
  activityLevel level;
  objective target;
  Gender g;
  string name, activeLevel;
  int age, gender, height, weight, levelN, targetN;
  cout << "Welcome to your UberEats preferred meals calculator" << endl;
  cout << "Tell us your name" << endl;
  cin >> name;
  cout << "Your age? (years, only numbers)" << endl;
  cin >> age;
  cout << "How tall are you? (centimeters, only numbers)"<< endl;
  cin >> height;
  cout << "How much do you weigh? (kilograms, only numbers)"<<endl;
  cin >> weight;
  cout << "On a scale from 1 to 5, how physically active do you consider yourself?" << endl;
  cin >> levelN;
  switch(levelN){
    case 1: level = BA; break;
    case 2: level = LA; break;
    case 3: level = MA; break;
    case 4: level = VA; break;
    case 5: level = XA; break;
    default: cout << "Invalid response, shutting down"; return 0;
  }
  cout << "Please type the number that you agree the most with." << endl;
  cout << "1. I want to lose weight" << endl;
  cout << "2. I want to build muscle" << endl;
  cout << "3. I want to eat healthy" << endl;
  cout << "4. I have no particular goals right now" << endl;
  cin >> targetN;
  switch(targetN){
    case 1: target = FIT; break;
    case 2: target = MUSCLE; break;
    case 3: target = OPT; break;
    case 4: target = ANY; break;    
    default: cout << "Invalid response, shutting down"; return 0;
  }
  cout << "And one last thing: are you a man or a woman?"<< endl;
  cout << "1. Man"<< endl;
  cout << "2. Woman"<< endl;
  cin >> gender;
  if (gender == 1){
    g = MALE;
  }
  else {
    g = FEMALE;
  }
  Human human(name,g,height,weight,age,level,target);
  cout << human.name << ", your daily intake is: " << human.dailyIntake << " and your TDEE is: " << human.tdee << endl;
  cout << "Your needs are:" << endl;
  cout << "Carbs: " << human.neededCarbs << "g or above." << endl;
  cout << "Fats: " << human.neededFats << "g or above." << endl;
  cout << "Proteins: " << human.neededProteins << "g or above." <<  endl;
  genetic(human);
  return 0;
}