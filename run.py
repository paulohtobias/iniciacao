from subprocess import check_output
import os
import numpy as np
from math import sqrt

# Entrada
inst = './instancias' + '/'

# Saida
pasta = 'resultados' + '/'

if not os.path.exists(pasta):
    os.makedirs(pasta)

# numero de testes
n = 10

# Para cada instancia
soma = 0
valores = []

# Fazer 10 execucoes
for j in range(1,n+1):

	# executa
	u = check_output(["./main.out", "testes/RedeArtigo/redeArtigo.txt", "testes/RedeArtigo/tripsArtigo.txt", "4"])

	print("output: <" + u + ">")

	# salva resultado em arquivo
	fout = open(pasta + "result_" + str(j) + ".txt", "w")
	fout.write(str(u))
	fout.close()

	# calculos
	soma = soma + float(u) 

# armazena os 10 valores
valores.append(float(u))

# media
media = soma/n
print ("media:", media)

# variancia
variancia = 0
for valor in valores:
	variancia = variancia + (valor - media)**2

variancia = variancia/n
print ("variancia: ", variancia)
#print np.var(valores)

# desvio
desvio = sqrt(variancia)
print ("desvio: ", desvio)
#print np.std(valores)

# IC de 95%
inferior = media - 2.262*(desvio/sqrt(n))
superior = media + 2.262*(desvio/sqrt(n))

# Imprime
print (">>> Instancia : " + str(inferior) + " <= " + str(media) + " <= " + str(superior) )
