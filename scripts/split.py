import fastaparser
import os

for seq in fastaparser.Reader(open('data/GRCh38_latest_genomic.fna'), parse_method='quick'):
    open(os.path.join('data', seq.header), 'w').write(seq.sequence)

target = 'NC_000006.12 Homo sapiens chromosome 6, GRCh38.p13 Primary Assembly'

l = open(os.path.join('data', f'>{target}')).read()
n = 100000
e = 1099999+1
r = l[n:e].lower()
print(set(r))
open('data/train', 'w').write(r)
n = 1100000
e = 2099999+1
r = l[n:e].lower()
open('data/test', 'w').write(r)
