#!/usr/bin/python
#-*- coding: utf-8 -*-

from random import uniform
from math import fabs
from math import cos
from math import pi
#from math import inf

DEBUG = False
INTERATIONS = 80000
ALFA= 0.01
C = 0.1
STEP_IND=0.1


MINIMUM_C  = 0.001
MAXIMUM_C = 0.999


MIN_WEIGHT = 1
MAX_WEIGHT = 1000

class FSSII():
    def __init__(self, number_fishes, beta , problem):
        self.number_fishes = number_fishes
        self.max_iteration = INTERATIONS
        self.problem = problem
        self._fishes = []
        self.best_fish = None         
        self.beta = beta
        self.fitness_best_fish = 0
        self.c = C

        for nparticle in range(number_fishes):
            fish = FishII(nparticle, self.beta, problem)            
#            fish.update_fitness()                      
            self._fishes.append(fish)

    def run(self):
        higher_delta_fitness = 0
        it = 1                
        prev_count_fish = curr_count_fish=0
        alfa = - ALFA
        #Local Search
        for _, fish in enumerate(self._fishes):
            fish.individual_movement(it)
            if (fish.delta_fitness > higher_delta_fitness):
                higher_delta_fitness = (fish.delta_fitness)
        
        #Alimentacao
        for fish in self._fishes:
            fish.calculate_weight_I(higher_delta_fitness)
            if (fish.delta_w > 0):
                curr_count_fish +=1
        
        prev_count_fish = curr_count_fish
        history = []
        while it < INTERATIONS:
            higher_delta_fitness = 0
            curr_count_fish = 0
            sum_delta_w = 0
            
#            self.c = C * ( 1 + alfa)
            self.c += self.c *  alfa

            if self.c < MINIMUM_C:
                self.c = MINIMUM_C;
            elif self.c > MAXIMUM_C:
                self.c = MAXIMUM_C;

            for _, fish in enumerate(self._fishes):
                fish.fish_displacement()
                fish.fitness_variation()
                
                if (fish.delta_fitness > higher_delta_fitness):
                    higher_delta_fitness = fish.delta_fitness
                                    
                fish.calculate_weight_II(higher_delta_fitness)                       
                sum_delta_w += fish.delta_w
                if (fish.delta_w > 0):
                    curr_count_fish +=1
            
            barycentre = self._calculate_barycentre()
            instintive_collective = self._calculate_instintive_collective()
            for fish in self._fishes:
                volitive_collective = fish.calculate_volitive_collective(sum_delta_w, barycentre, self.c)
                fish.update_position(instintive_collective, volitive_collective, self.c)                            
                    
                if (fish.current_fitness > self.fitness_best_fish):
                    # print 'Modificando o melhor fitness'
                    self.fitness_best_fish = fish.current_fitness
                    self.best_fish = fish
                
                """
                print fish.label
                print 'prev position', fish.prev_position
                print 'curr position', fish.current_position
                print 'delta_x', fish.delta_x
                print 'delta_w', fish.delta_w
                print 'delta_f', fish.delta_fitness
                """
            if curr_count_fish < prev_count_fish:
                alfa *= -1
            
            prev_count_fish = curr_count_fish
                    
            # print 'Iteracao: %d' % it
            # #print "*******************************************************"
            # print self.best_fish
            line = ("%.15f\n" % (1.0/self.best_fish.current_fitness))
            history.append(line)            
#            print "*******************************************************"
            it+=1
            if (self.best_fish.current_fitness == float('inf')):
                break
        print self.best_fish
        return history

    def _calculate_instintive_collective(self):
        instintive_collective = [0] * self.problem.dimensions
        sum_prod = [0] * self.problem.dimensions
        sum_weight_now = 0
        
        for fish in self._fishes:
            for pos, _ in enumerate(fish.delta_x):
                sum_prod[pos] += fish.delta_x[pos] * fish.weigth
            sum_weight_now += fish.weigth
        
        constant = self.c * uniform(0,1)    
        for pos, _ in enumerate(sum_prod):
            instintive_collective[pos] = constant * (sum_prod[pos]/sum_weight_now)
            
        return instintive_collective
           
    def _calculate_barycentre(self):
        school_barycentre = [0] * self.problem.dimensions
        sum_prod = [0] * self.problem.dimensions
        sum_weight_now = 0
        
        for fish in self._fishes:            
            for pos,value in enumerate(fish.current_position):
                sum_prod[pos] += fish.current_position[pos] * fish.weigth
            sum_weight_now += fish.weigth
        
        for pos,value in enumerate(sum_prod):
            school_barycentre[pos] = value / sum_weight_now
            
        return school_barycentre

    def __str__(self):
        result=''
        for fish in self._fishes:
            result+=str(fish)
        return result
    
class FishII():
    def __init__(self, label, beta, problem):
        self.label = label
        self.problem = problem
        self.current_position =[]
        self.beta = beta 
         
        if (not self.current_position):
            while len(self.current_position) < self.problem.dimensions:
                    value = uniform(self.problem.vMin, self.problem.vMax)
                    self.current_position.append(value)

            self.neighbor_position = self.current_position[:]
            self.prev_position = self.current_position[:]
            self.prev_fitness = self.neighbor_fitness = self.current_fitness = self.calculate_fitness(self.current_position)
            
        print self.current_position             
        self.weigth =  uniform(300,600); # All the fish are born with weight like that
        
        self.old_weigth = self.weigth 
        self.delta_x =  len(self.neighbor_position) * [0]
        
        
    def calculate_weight_I(self, delta_fitness_max):
        
        if delta_fitness_max != 0:
            fitness_gain_normalized = (self.delta_fitness * 1.0) / delta_fitness_max
            new_weigth = self.weigth + fitness_gain_normalized
        
            if (new_weigth > MAX_WEIGHT):
                new_weigth = MAX_WEIGHT
            if (new_weigth < MIN_WEIGHT):
                new_weigth = MIN_WEIGHT
        
            
            self.old_weigth = self.weigth
            self.weigth = new_weigth
        
        self.delta_w = self.weigth - self.old_weigth

    def calculate_fitness(self, positions):
        return self.problem.evaluate(positions)   
        
    def individual_movement(self, iteration, step_size=0.1):
        pos = 0
        self.neighbor_position = self.current_position[:]
        
        while pos < len(self.neighbor_position):
            self.delta_x[pos] = uniform(-1, 1) * STEP_IND
            pos+=1        
        
        pos = 0
        while pos < len(self.neighbor_position):
            
            self.neighbor_position[pos] = self.neighbor_position[pos] + self.delta_x[pos]
             
            if (self.neighbor_position[pos] < self.problem.vMin):
                self.neighbor_position[pos] = self.problem.vMin
            if (self.neighbor_position[pos] > self.problem.vMax):
                self.neighbor_position[pos] = self.problem.vMax            
            pos+=1
        
        self.neighbor_fitness = self.calculate_fitness(self.neighbor_position)
        
        #if self.neighbor_fitness > self.current_fitness:
        self.prev_fitness = self.current_fitness
        self.prev_position = self.current_position[:]            
        self.current_fitness = self.neighbor_fitness
        self.current_position = self.neighbor_position[:]
        
        self.delta_fitness = self.current_fitness - self.prev_fitness
        
    def fish_displacement(self):
        self.delta_x = [0] * len(self.current_position)
        for pos, _ in enumerate(self.delta_x):
            self.delta_x[pos] = self.current_position[pos] - self.prev_position[pos]
                
    def fitness_variation(self):        
        self.delta_fitness = self.current_fitness - self.prev_fitness
    
    def calculate_weight_II(self, max_delta_fitness):
        
        if max_delta_fitness == 0:
            max_delta_fitness = 1
            
        new_weigth = self.weigth + self.delta_fitness/max_delta_fitness
        self.old_weigth = self.weigth
        self.weigth = new_weigth
        self.delta_w = self.weigth - self.old_weigth
        
    def update_position(self, instintive_collective, volitive_collective, c):
        prod_delta_x = [0] * len(self.delta_x)
        new_position = [0] * len(self.current_position)
        
        for pos, _ in enumerate(self.delta_x):
            prod_delta_x[pos] = self.beta * c * self.delta_x[pos]
        
        for pos, _ in enumerate(new_position):
            new_position[pos] = -1 * self.current_position[pos] + prod_delta_x[pos] + instintive_collective[pos] + volitive_collective[pos]
            
            if (new_position[pos] < self.problem.vMin):
                new_position[pos] = self.problem.vMin
            if (new_position[pos] > self.problem.vMax):
                new_position[pos] = self.problem.vMax
        
        new_fitness = self.calculate_fitness(new_position)
        
        #if new_fitness >= self.current_fitness:
        self.prev_position = self.current_position[:]
        self.prev_fitness = self.current_fitness            
        self.current_position = new_position[:]  
        self.current_fitness = new_fitness
    
    def calculate_volitive_collective(self, sum_delta_w, barycentre, c):
        volitive_collective = [0] * self.problem.dimensions
        
        if sum_delta_w >= 0:
            signal= +1
        else:
            signal= -1
        
        constant = c * uniform(0,1) * signal
        
        for pos, _ in enumerate(barycentre):
            volitive_collective[pos] = constant * (self.current_position[pos] - barycentre[pos])
       
        return volitive_collective    
        
    def __str__(self):
        return "(%02d) Current Fitness = %10.20f | Real Fitness = %10.20f\nCurrent position = %s \n" % (self.label, self.current_fitness, 1.0/self.current_fitness, str(self.current_position))
    

class Problem():
    
    def __init__(self, name, dimensions, vMax, vMin):
        self.name = name
        self.dimensions = dimensions
        self.min = 0
        self.max = 0
        self.vMax = vMax
        self.vMin = vMin        
    
    def evaluate(self, postions):
        raise NotImplementedError 

class Esfera(Problem):    
    def __init__(self):
        Problem.__init__(self, "Esfera", 30, 100, -100)
        
    def evaluate(self, postions):
        total=0
        
        for position in postions:
            total += position ** 2
        
        #print 'Evaluate', 1.0/total
        if total == 0:
            return 1000000
        else:    
            return 1.0/total
 
class Rastrign(Problem):
    def __init__(self):
        Problem.__init__(self, "Rastrign", 30, 5.12, -5.12)
    
    def evaluate(self, positions):
        A = 10
        total = A * len(positions)
        for position in positions:
            total +=  position**2 - 10 * cos(2.0 * pi * position)
        
        if total == 0:
            return 10000000
        else:
            return 1.0/total

class Rosenbrock(Problem):
    def __init__(self):
        Problem.__init__(self, "Rosenbrock", 30, 30, -30)
    
    def evaluate(self, positions):
        auxSum = 0
        for pos in range(0, len(positions)-1):
            aux = 100*((positions[pos+1] - (positions[pos])**2)**2)
            aux += (1.0 - positions[pos])**2
            auxSum += aux
        
        if auxSum == 0:
            return 10000000
        else:
            return 1.0/auxSum


if __name__ == '__main__':
    rastrigin = Rastrign()
    esfera = Esfera()
    rosenbrock = Rosenbrock()
    fss = FSSII(30, 0.4, rosenbrock)    
    history=fss.run()
    convergencia = open('results_fss2_rastrigin.txt', 'w')
    convergencia.writelines(history)
    #print fss