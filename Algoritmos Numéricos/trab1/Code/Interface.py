#!/usr/bin/python
# -*- coding: utf-8 -*-

# Autor: Vinicius Ferraço Arruda
# Email: viniciusferracoarruda@gmail.com
# Trabalho de Algoritmos Numéricos I: Interpolação por partes via splines cúbicas
# Professora: Cláudia Galarda Varassin
# Instituição: Universidade Federal do Espírito Santo
# Data: 23 de novembro de 2016


from Spline import Spline

class Interface:
	# Método de inicialização da interface com o usuário
	def start(self):
		while True:
			choice = self._getChoice(self._getMenu())   
			(n,X,Y,z,m) = self._getDataByChoice(choice)  
			s = Spline(X,Y)
			print '%.20f' % s.getY(z)
			s.savePlot('grafico',m)

	def _getMenu(self):
		return "__________Menu___________\n" + \
		       "1 - Ler dados via teclado\n" + \
			   "2 - Ler dados via arquivo\n" + \
		       "3 - Sair\n" + \
		       "Escolha: "

	def _getChoice(self, message):
		return int(raw_input(message))

	def _getDataByChoice(self, choice):             
		if choice == 1:   return self._getDataByKeyboard()
		elif choice == 2: return self._getDataByFile()
		elif choice == 3: exit(0)

	def _getDataByKeyboard(self): 
		n = int(raw_input())
		X = map(float, raw_input().split())
		Y = map(float, raw_input().split())
		z = float(raw_input())
		m = 50
		return (n, X, Y, z, m)

	def _getFileMenu(self):
		return "___________Menu___________\n" + \
		       "1 - Ler arquivo dados1.txt\n" + \
		       "2 - Ler arquivo dados2.txt\n" + \
		       "3 - Ler arquivo dados3.txt\n" + \
		       "4 - Sair\n" + \
		       "Escolha: "

	def _getDataByFileChoice(self, choice):
		if choice == 1:   return self._getDataFromFile("dados1.txt") + (50,)
		elif choice == 2: return self._getDataFromFile("dados2.txt") + (100,)
		elif choice == 3: return self._getDataFromFile("dados3.txt") + (100,)
		elif choice == 4: exit(0)

	def _getDataFromFile(self, filename):
		lines = [line.rstrip('\n') for line in open(filename)]
		n = int(lines[0])
		X = map(float, lines[1].split())
		Y = map(float, lines[2].split())
		z = float(lines[3])
		return (n, X, Y, z)

	def _getDataByFile(self):  
		choice = self._getChoice(self._getFileMenu())
		return self._getDataByFileChoice(choice)
