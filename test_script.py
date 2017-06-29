import subprocess
import os
from random import randint
import time

message = "MC833"
procs = []
for i in range(0,8):
    try:
        rand_type = str(randint(0,3))
        rand_size = str(randint(1,5))
        rand_speed = str(randint(1,9))
        rand_pos = str(randint(1,10))
        rand_dir = str(randint(0,1))
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
        print("Running clients")
        time.sleep(5)
    except KeyboardInterrupt:
        for proc in procs:
            proc.kill()
        exit()

    
