#In[]
train=open("data/train").read()

#In[]
import mchmm as mc

a = mc.MarkovChain().from_data(train)

#In[]
import numpy as np

print(np.sum(a.observed_matrix))
print(a.states)
print(a.observed_p_matrix)
