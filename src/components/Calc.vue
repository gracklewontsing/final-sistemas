<script setup>
import { ref, computed, reactive } from 'vue'
const POPULATION_TYPE = 25;
const POPULATION_SIZE = 10;
let population, new_generation, child, aptitude, parent_a, parent_b
new_generation = new Array(POPULATION_SIZE)
child = new Array(POPULATION_TYPE)
parent_a = new Array(POPULATION_TYPE)
parent_b = new Array(POPULATION_TYPE)
for(let i=0; i<POPULATION_SIZE;i++){
  new_generation[i] = new Array(25)
}
// let population[POPULATION_SIZE][POPULATION_TYPE]
// let new_generation[POPULATION_SIZE][POPULATION_TYPE]
// let child[POPULATION_TYPE]
// let aptitude[POPULATION_SIZE]
// let parent_a[POPULATION_TYPE]
// let parent_b[POPULATION_TYPE]

class FoodItem {
  //calories are obtained  as follows
  // carbs: 4 calories per gram, eg : carb = 10g, calories = 40
  // fats: 9 cals per gram
  // protein: 4 cals per gram  
    constructor(n, c, f, p){
      this.name = n;
      this.carbs = c;
      this.fats = f;
      this.proteins = p;
      this.calories = (this.proteins * 4) + (this.fats * 9) + (this.carbs * 4);
    }
};
class Human {
  // string name;
  // Gender gender;
  // let height; //cm
  // let weight; //kg
  // let age; //years
  // activityLevel level; //now as barely, lightly, moderately, very and extremely active  
  // objective target;
  // let dailyIntake;
  // let tdee;
  // let neededCarbs = 0;
  // let neededFats = 0;
  // let neededProteins = 0;
  constructor(n, g, h, w, a, l, t){
    this.name = n;
    this.gender = g;
    this.height = h;
    this.weight = w;
    this.age = a;
    this.level = l;
    this.target = t;
      // daily calorie letake needs are as follows
      // Men: calories/day = 10 x weight (kg) + 6.25 x height (cm) – 5 x age (y) + 5
      // Women: calories/day = 10 x weight (kg) + 6.25 x height (cm) – 5 x age (y) – 161
    if(this.gender == "male"){
      this.dailyIntake = (10 * this.weight)+(6.25*this.height)-(5*this.age)+5;
    }
    else {
      this.dailyIntake = (10 * this.weight)+(6.25*this.height)-(5*this.age)-161;
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
    switch (this.level) {
      case "Barely": this.tdee = this.dailyIntake * 1.2; break;
      case "Slightly": this.tdee = this.dailyIntake * 1.375; break;
      case "Moderately": this.tdee = this.dailyIntake * 1.55; break;
      case "Highly": this.tdee = this.dailyIntake * 1.725; break;
      case "Very Highly": this.tdee = this.dailyIntake * 1.9; break;
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
      //micronutrients (minerals, vitamins) are excluded as the needed letake will be allegedly consumed from most foods
      //although meals have also a bias per period, they will be excluded
    switch (this.target) { //set lower limits
      case "Lose weight": this.neededFats = (this.tdee * .30)/9; this.neededProteins = (this.tdee * .35)/4; break; //as long as fats and proteins exceed these amounts, it is accepted
      case "Build muscle": this.neededProteins = (this.tdee* .40)/4; break; //as long as proteins equal or exceed this amount, it is accepted
      case "Eat healthy": this.neededCarbs = (this.tdee*.30)/4; this.neededFats = (this.tdee * .25)/9; this.neededProteins = (this.tdee*.35)/4; break; //as long as the lower limits are met or exceeded, it is accepted
      case "I have no particular goals right now": this.neededCarbs = (this.tdee*.35)/4; this.neededFats = (this.tdee * .30)/9; this.neededProteins = (this.tdee*.35)/4; break;
    }
  }    
}
var menu = [
  new FoodItem("Chicken Sandwich", 39, 21, 30),
  new FoodItem("Lemnas Bread (2 slice)", 34,2,10),
  new FoodItem("Gray Fruit",25,5,24),
  new FoodItem("Salad(dressed)",12,24,8),
  new FoodItem("Lum Berry",10,5,0),
  new FoodItem("Apple",10,2,0),
  new FoodItem("Normal Cheez",4,15,24),
  new FoodItem("Pork Rinds",0,27,3),
  new FoodItem("Boneless Chicken Wings",20,20,40),
  new FoodItem("Snickers",35,14,4),
  new FoodItem("Ramen",111,35,48),
  new FoodItem("Pepperoni Pizza (slice)",35,21,13),
  new FoodItem("Pepperoni Pizza (4 slices)",140,82,52),
  new FoodItem("Burger, generic",40,27,34),
  new FoodItem("Steaks", 0,54,94),
  new FoodItem("Chicken Nuggets (10 pz)", 23, 29, 25),
  new FoodItem("Fish Fillet", 0, 5,44),
  new FoodItem("Salmon Fillet",0,20,50),
  new FoodItem("Ham Sandwich", 48,14,22),
  new FoodItem("Fried eggs",2,28,26),
  new FoodItem("Bacon(4 slices)", 0,16,16),
  new FoodItem("Fried rice(cup)", 45,4,6),
  new FoodItem("Chicken Breasts", 0,11,71),
  new FoodItem("Tacos al pastor(4)", 8,20,50),
  new FoodItem("Cereal bowl", 42,4,7)
];

const data = reactive({
  name:'Jesus',
  gender:'Male',
  height:'179',
  weight:'80',
  age:'28',
  levelN:'Barely',
  targetN:'Lose weight',
  icals:0,
  icarbs:0,
  ifats:0,
  iproteins:0,
  showModalCalc: false,
  showModal:false,
  user:''
})
let list = []

async function calculate(){  
  data.showModal = true  
  await new Promise(r => setTimeout(r, 2000));
  data.user= new Human(data.name,data.gender,data.height,data.weight,data.age,data.levelN,data.targetN)
  //console.log(human)
  genetic(data.user)
}

function close(){
  data.showModalCalc = false
  data.icals = 0
  data.icarbs=0
  data.iproteins=0
  data.ifats=0  
  while (list.length > 0){
    list.pop()
  }
}
// mutation in a genome
function mutate()
{    
    let r = Math.random() * POPULATION_TYPE;
    if(child[r] == 0){
      child[r] = 1;
    }
    else{
      child[r]=0;
    }        
}
function populate() {
  population = new Array(POPULATION_SIZE)
  for(let i = 0; i < POPULATION_SIZE; i++){
    population[i] = new Array(POPULATION_TYPE);
    for(let j = 0; j < POPULATION_TYPE; j++){
      population[i][j] = (Math.floor(Math.random() * 2));
      
    }
  }
  //console.log(population)
}
function calcNutrients(user){      
  let calorieCount=0;
  let carbCount=0;
  let fatCount=0;
  let proteinCount=0;    
  aptitude = new Array(POPULATION_SIZE);
  for(let i = 0; i<POPULATION_SIZE; i++){
    aptitude[i] = 0;
    for(let j = 0; j < POPULATION_TYPE;j++){    
      if(population[i][j] == 1){
        //cout<<menu[j].name<<endl;
        calorieCount += menu[j].calories;
        carbCount += menu[j].carbs;
        fatCount += menu[j].fats;
        proteinCount += menu[j].proteins;
      }
    }
    if(calorieCount != user.tdee){
      aptitude[i] += Math.abs((user.tdee-calorieCount));        
    }
    if(Math.abs(carbCount-user.neededCarbs)>15 && user.neededCarbs!=0){
      aptitude[i] += Math.abs((user.neededCarbs-carbCount));
    }
    if(fatCount < user.neededFats && user.neededFats!=0){
      aptitude[i] += Math.abs((user.neededFats-fatCount))*.7;
    }
    if(Math.abs(proteinCount - user.neededProteins)>15  && user.neededProteins!=0){
      aptitude[i] += Math.abs((user.neededProteins-proteinCount));      
    }
    calorieCount=0;
    carbCount=0;
    fatCount=0;
    proteinCount=0;
    //cout << aptitude[i]<<endl;
  }
}
function cross(){
  let random_a = Math.random()* POPULATION_TYPE;
  let random_b = Math.random()* POPULATION_TYPE;
  while(random_b == random_a){
    random_b = Math.random()* POPULATION_TYPE;
  }
  //start as parent b
  for(let i = 0; i<POPULATION_TYPE; i++){
    child[i] = parent_b[i];
  } 
  //leternal range eg.: { 0, 1, 1,[0,1,0,1],0,1 }
  if(random_a < random_b){
    //cross genes from a to child
    for(let i = random_a; i<random_b;i++){
      child[i] = parent_a[i];
    }
  }
  //out of bound range eg.: { 0, 1, 1],0,1,0,1,[0,1 }
  else if(random_a > random_b) {
    //cross genes from a to child
    for(let i = random_a; i<POPULATION_TYPE;i++){
      child[i] = parent_a[i];
    }
    for(let i = 0; i<random_b;i++){
      child[i] = parent_a[i];
    }
  }  
}
function selection_cross_mutate(){
  let random_a = Math.floor(Math.random() * POPULATION_SIZE);  
  let random_b = Math.floor(Math.random() * POPULATION_SIZE);  
  
  while(random_b == random_a){
    random_b = Math.floor(Math.random() * POPULATION_SIZE);
  }
  let fitter=999999;
  let fitter_pos=30;
  let fittest=999999;
  let fittest_pos=30;
  //select 2 fittest
  for(let i = 0; i<POPULATION_SIZE; i++){    
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
  for(let j=0; j<POPULATION_TYPE;j++){ 
    new_generation[0][j] = population[fittest_pos][j];
    new_generation[1][j] = population[fitter_pos][j];      
  }  
  //console.log(new_generation)
  //select 8 others randomly for reproduction
  for(let k = 2; k<POPULATION_SIZE;k++){
    //select parents randomly
    for(let i = 0; i<POPULATION_TYPE; i++){      
      parent_a[i] = population[random_a][i];
      parent_b[i] = population[random_b][i];    
    }

    //cross parents to a child
    cross();  

    //mutate the childs genomes
    if(((Math.random()*10) +1) <= 2)
      mutate();           
      
    //imprlet child leto new generation
    for(let j=0; j<POPULATION_TYPE;j++) 
      new_generation[k][j] = child[j];
  }        
}
function overwrite(){
  // for (let i=0;i<POPULATION_SIZE;i++){
  //   for (let j =0; j<POPULATION_TYPE;j++){
  //     population[i][j]=new_generation[i][j];
  //   }
  // }
  population = new_generation
}
function genetic(user){
  populate();
  let fitness=999999;
  let generations = 0;
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
    for(let i = 0; i<POPULATION_SIZE;i++){
      if(aptitude[i]<fitness){
        fitness = aptitude[i];
      }
    }
    console.log("GENERATION ", generations, " -> FITNESS: ", fitness);
  }    
  let best;
  for (let i = 0; i<POPULATION_SIZE;i++){
    if(fitness == aptitude[i]){
      best = i;
    }
  }
    
  for (let i = 0; i<POPULATION_TYPE;i++){    
    if(population[best][i] == 1){            
      list.push(menu[i])
      console.log(menu[i])
      data.icals += menu[i].calories;
      data.icarbs += menu[i].carbs;
      data.ifats += menu[i].fats;
      data.iproteins += menu[i].proteins;
    }    
  }  
  data.showModal = false
  data.showModalCalc = true
  // cout << cals << " total calories against "<< user.tdee<< " needed calories."<<endl;
  // cout<< carbs << "g of total carbs against "<< user.neededCarbs<<"g needed carbs."<<endl;
  // cout<< fats <<  "g of total fats against "<< user.neededFats<<"g needed fats."<<endl;
  // cout<< prots <<  "g of total proteins against "<< user.neededProteins<<"g needed proteins."<<endl;
}
</script>

<template>
 <div>
   <va-form>
     <h6 class="display-6 mb-4 text--left">Tell us your name... </h6>
    <va-input
      class="mb-4"
      v-model="data.name"
      label="Name"
      placeholder="John Smith"
    />
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">Are you a man or a woman? </h6>
    <va-option-list
      type="radio"
      :options="['Male', 'Female']"
      v-model="data.gender"
    ></va-option-list>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">How tall are you?(cm) </h6>
    <va-input
      class="mb-4"
      v-model="data.height"
      label="Height"
      placeholder="0-1000"
    ></va-input>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">How much do you weigh? (kg) </h6>
    <va-input
      class="mb-4"
      v-model="data.weight"
      label="Weight"
      placeholder="0-1000"
    ></va-input>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">How old are you?(years) </h6>
    <va-input
      class="mb-4"
      v-model="data.age"
      label="Age"
      placeholder="0-100"
    ></va-input>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">How much do you exercise?</h6>
    <va-option-list
      type="radio"
      :options="['Barely', 'Slightly', 'Moderately', 'Highly', 'Very Highly']"
      v-model="data.levelN"
    ></va-option-list>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">Lastly, what are your objectives for a diet?</h6>
    <va-option-list
      type="radio"
      :options="['Lose weight', 'Build muscle', 'Eat healthy', 'I have no particular goals right now']"
      v-model="data.targetN"
    ></va-option-list>
    <va-divider inset />
    <h6 class="display-6 mb-4 text--left">Once everything is filled, click here to calculate a diet:</h6>
    <va-button @click="calculate()"> Calculate! </va-button>
    </va-form>
    <va-modal
      v-model="data.showModal"
      hide-default-actions
      overlay-opacity="0.2"
    >
      <template #header>
        <h6 class="display-6">Calculating...</h6>
      </template>
      <slot >
        <va-inner-loading :loading="true" class="my-3" icon="autorenew">                   
        </va-inner-loading>
        <p class="mt-4">Preparing diet... </p>
      </slot>   
    </va-modal>
    <va-modal v-model="data.showModalCalc"
      hide-default-actions
      overlay-opacity="0.2">   
      <template #header>
        <h2 class="display-2">Diet found.</h2>
      </template>
      <slot>
        <div>We found an appropriate diet for you!</div>
          <div class="table-responsive">
            <table class="va-table">
              <thead>
                <tr>
                  <th>Food</th>
                  <th>Calories</th>
                  <th>Carbs</th>
                  <th>Fats</th>
                  <th>Proteins</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="item in list" :key="item.id">
                  <td>{{item.name}}</td>
                  <td>{{item.calories}}</td>
                  <td>{{item.carbs}}</td>
                  <td>{{item.fats}}</td>
                  <td>{{item.proteins}}</td>
                </tr>
              </tbody>
            </table>
          </div>          
          <h6 class="display-6">Your totals are...</h6>
          <p>{{data.icals}} total calories against {{data.user.tdee}} needed calories.</p>
          <p>{{data.icarbs}} total carbs against {{data.user.neededCarbs}} needed carbs.</p>
          <p>{{data.ifats}} total fats against {{data.user.neededFats}} needed fats.</p>
          <p>{{data.iproteins}} total proteins against {{data.user.neededProteins}} needed proteins.</p>
      </slot>
      <template #footer>
        <va-button @click="close()" color="danger" class="center mt-3"> Close </va-button>        
      </template>
    </va-modal>
 </div>
</template>
<style scoped>
a {
  color: #42b983;
}
.center {
  margin-left: 40%;
  margin-right: 40%;
}
</style>
