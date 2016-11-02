import numpy as np
import matplotlib.pyplot as plt
import math
from sys import stdin


def link_points(i,j):
    x_points=np.array([x[i], x[j]])
    y_points=np.array([y[i], y[j]])
    plt.plot(x_points, y_points, color = 'brown', marker = 'o')

def distance_city(i,j):
    return math.sqrt(pow((x[i] - x[j]),2) + pow((y[i] - y[j]),2))

def buildtours():
    basic_tour = []
    improved_tour = []
    lines = stdin.readlines()
    for city in lines[0].split(" ")[0:-1]:
        basic_tour.append(city)
    for city in lines[1].split(" ")[0:-1]:
        improved_tour.append(city)
    return basic_tour, improved_tour

def draw_tour(tour, id):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylim(0,100)
    plt.plot(x, y, 'ro')
    it = 0
    for i,j in zip(x,y):
        ax.annotate(str(it),xy=(i-1,j-1))
        it = it+1
    tour_distance = 0
    for i in range(len(tour)-1):
        tour_distance += distance_city(int(tour[i]),int(tour[i+1]))
        link_points(int(tour[i]),int(tour[i+1]))
    link_points(tour[0], tour[-1])
    tour_distance += distance_city(tour[0],tour[-1])
    if(id==1):
        ax.annotate("Basic tour",xy=(1,5))
    elif(id==2):
        ax.annotate("2-opt tour",xy=(1,5))
    ax.annotate("Tour distance: " + str(tour_distance),xy=(1,1))


x_array = []
y_array = []

for line in open("tsp.in").xreadlines():
    x_array.append(float(line.split(" ")[0]))
    y_array.append(float(line.split(" ")[1]))

x = np.array(x_array)
y = np.array(y_array)

basic_tour, improved_tour = buildtours()
print "Basic tour "
print basic_tour
print "Improved tour"
print improved_tour
draw_tour(basic_tour, 1)
draw_tour(improved_tour, 2)
plt.show()
