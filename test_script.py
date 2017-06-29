#run with python3

import subprocess
import os
from random import randint
import time

message = "MC833"
procs = []

## Start the server
p = subprocess.Popen("./server", shell=True)
procs.append(p)

## Start the clients with random informations
for i in range(0,5):
    try:
        rand_type = str(1)
        rand_size = str(randint(1,5))
        rand_speed_value = randint(1,5)
        rand_pos_value = randint(-50,50)
        rand_pos = str(rand_pos_value)
        rand_dir = str(i % 2)

        if rand_pos_value > 0: 
            rand_speed_value = -1 * rand_speed_value
        
        rand_speed = str(rand_speed_value)

        print("./client "+rand_type+" "+rand_size+" "+rand_speed+" "+rand_pos+" "+rand_dir+" "+message)
        p = subprocess.Popen("./client "+rand_type+" "+rand_size+" "+rand_speed+" "+rand_pos+" "+rand_dir+" "+message, shell=True)
        procs.append(p)
        time.sleep(3)
    except KeyboardInterrupt:
        for proc in procs:
            proc.kill()
        exit()

while True:
    try:
        time.sleep(5)
    except KeyboardInterrupt:
        for proc in procs:
            proc.kill()
        exit()
