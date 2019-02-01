#!/usr/bin/python
# -*- coding: utf-8 -*-

# Autor: Vinicius Ferraço Arruda
# Email: viniciusferracoarruda@gmail.com
# Trabalho de Algoritmos Numéricos I: Interpolação por partes via splines cúbicas
# Professora: Cláudia Galarda Varassin
# Instituição: Universidade Federal do Espírito Santo
# Data: 23 de novembro de 2016


from __future__ import division
import matplotlib.pyplot as plt


class Spline:

	def __init__(self,x,y):                                     
		self.x = [float(xx) for xx in x]                         # Converte todos os valores da lista x para float
		self.y = [float(yy) for yy in y]                        
		self.c = self._getCoeficients()   

	def getY(self,xu):                                           # Avalia xu à sua respectiva spline
		for i in range(0, len(self.x)-1):
			if self.x[i] <= xu <= self.x[i+1]:                   # Procura de qual spline xu pertence
				h = xu - self.x[i]                               # Avalia o valor de xu à sua respectiva spline
				return self.c[0][i]*(h**3) + self.c[1][i]*(h**2) + self.c[2][i]*h + self.c[3][i]

	def savePlot(self,filename,m):                               # Salva o gráfico e os m pontos com seus respectivos mapeamentos em um arquivo
		xu = self._frange(self.x[0],self.x[-1],m)                # Pega m pontos
		yu = [self.getY(xxu) for xxu in xu]                      # Avalia os m pontos 

		plt.plot(xu, yu)                                         # Plota o gráfico e o salva em um arquivo png
		plt.plot(self.x, self.y, 'co')
		plt.tight_layout()                                       
		plt.savefig(filename + '.png')
		plt.close('all')

		with open(filename + '.txt', 'w') as f:                  # Escreve em um arquivo os valores de m e seus valores mapeados
			for (xxu, yyu) in zip(xu, yu):                      
	  			f.write("%.20f %.20f\n" % (xxu, yyu)) 

	def _getTridiagSys(self,h,dif):                              # Monta o sistema tridiagonal
		e = h[1:-1]                                              # Valoes abaixo da diagonal         
		f = [(a+b)*2.0 for a,b in zip(h[:-1], h[1:])]            # Valores da diagonal
		g = h[1:-1]                                              # Valores acima da diagonal
		r = [(a-b)*6.0 for a,b in zip(dif[1:], dif[:-1])]        # Imagem do sistema
		return (e,f,g,r)

	def _getResTridiagSys(self,e,f,g,r):                         # Resolve o sistema trigiagonal            
		for i in range(0,len(e)):                                # Decomposição
			e[i] = e[i] / f[i]                                   # 'e' agora é o multiplicador
			f[i+1] = f[i+1] - e[i] * g[i]                        # Atualiza os valores da diagonal

		for i in range(0,len(e)):                                # Substituições sucessivas
			r[i+1] = r[i+1] - e[i] * r[i]                        # Atualiza os valores da imagem

		n = len(f)                                               # Substituições retroativas
		s2d = [0.0] * n                                       
		s2d[n-1] = r[n-1]/f[n-1]
		for i in range(n-2,-1,-1):                               
			s2d[i] = (r[i] - g[i] * s2d[i+1])/f[i]               # Encontra a solução de cada derivada
		
		return s2d                                               # Retorna a solução (as derivadas segunda)

	def _getCoeficients(self):                                   # Encontra os coeficientes das splines
		h = [(a-b) for a,b in zip(self.x[1:],self.x[:-1])]       # Calcula h (x_{i+1} - x_i)
		dif = [((a-b)/c) for a,b,c in zip(self.y[1:],self.y[:-1],h)]  # Calcula as diferenças divididas ((x_{i+1} - x_i) / h_i)
		(e,f,g,r) = self._getTridiagSys(h,dif)                        # Monta o sistema de n-1 equações
		s2d = [0.0] + self._getResTridiagSys(e,f,g,r) + [0.0]         # Encontra as derivadas, e adiciona as derivadas de s_0 e s_n
		c4 = self.y[:-1]                                              # Calcula os 4n coeficientes
		c2 = [(a/2.0) for a in s2d[:-1]]                         
		c1 = [((a-b)/(c*6.0)) for a,b,c in zip(s2d[1:],s2d[:-1],h)] 
		c3 = [(a-((b+c*2.0)/6.0)*d) for a,b,c,d in zip(dif,s2d[1:],s2d[:-1],h)] 
		return (c1,c2,c3,c4)

	def _frange(self,head,tail,m):                               # Retorna m valores de ponto flutuante
		l = [head] * m                                           # no intervalo fechado [head, tail]
		jump = (tail-head)/(m-1)
		for i in range(1, m-1):
			l[i] = l[i-1] + jump
		l[m-1] = tail
		return l
