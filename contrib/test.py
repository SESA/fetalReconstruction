import os

THREADS=[1,2,3,4,5]
ITERATIONS=[8]#1,2,4]

def parse_tmp(dataset, threads, iterations):
    tmp = open('tmp', 'r')
    csv = dataset + "," + str(threads) + "," + str(iterations) + ","
    log = open('results.csv', 'a')
    for line in tmp:
        line = line.rstrip()
        if line:
            s = line.split();
            csv = csv + s[-1] + "," 
    csv = csv[:-1]
    print(csv)
    log.write(csv + "\n")

def run_test(dataset, threads, iterations):
    
    if dataset == "small":
        out = os.system("./contrib/small.sh " + str(threads) + " " +
                str(iterations)) 
    else:
        out = os.system("./contrib/large.sh " + str(threads) + " " +
                str(iterations))

    parse_tmp(dataset, threads, iterations)

for iteration in ITERATIONS:
    for thread in THREADS:
        run_test("small", thread, iteration)
        #run_test("large", thread, iteration)
