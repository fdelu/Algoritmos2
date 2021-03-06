import grafo_util
from lectura_datos import ID_CANCION, ID_USUARIO
import heapq
from mensajes import *

# Devuelve una lista con los top k elementos más grandes de la lista pasada,
# quitándolos de ella.
# Le aplica heapify a la lista, a menos que es_heap sea True en cuyo caso
# se asume que ya cumple condición de heap.
def _top_k(lista, k, es_heap = False):
	if not es_heap:
		heapq.heapify(lista)
	top = []
	for i in range(k):
		top.append(heapq.heappop(lista))

	return top

# Arma el mensaje a imprimir correspondiente a la función de camino.
# Recibe una lista de aristas ((v1, v2, peso)) con v = (ID_CANCION/ID_USUARIO, nombre)
# La lista debe tener longitud impar
def _mensaje_camino(camino):
	msj = [None]*(len(camino)*4)
	msj[::4] = [x[0][1] for x in camino] # Agrego los nombres de los v1
	msj[2::4] = [x[2] for x in camino] # Agrego los pesos (playlists)
	msj[1::2] = CAM_MENSAJES*int(len(camino)/2) # Agrego las partes constantes del mensaje
	msj.append(camino[-1][1][1]) # Agrego el nombre del ultimo v2
	return msj

def camino(parametros, grafo_usuarios):
	origen, _, destino = parametros.partition(SEPARADOR_CANCIONES)
	origen = (ID_CANCION, origen)
	destino = (ID_CANCION, destino)
	if origen not in grafo_usuarios or destino not in grafo_usuarios:
		print(CAM_ERROR)
		return

	camino_min = grafo_util.camino_minimo(grafo_usuarios, origen, destino, con_peso = True)
	if len(camino_min) == 0:
		print(SIN_RECORRIDO)
		return

	print(*_mensaje_camino(camino_min), sep = FLECHA)

def ciclo(parametros, grafo_canciones):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit() or int(n) <= 0:
		print(NO_NUMERO.format(n))
		return
	if cancion not in grafo_canciones:
		print(NO_CANCION.format(cancion))
		return
	n = int(n)

	lista_ciclo = grafo_util.ciclo_n(grafo_canciones, cancion, n)
	if len(lista_ciclo) == 0:
		print(SIN_RECORRIDO)
		return

	print(*lista_ciclo, sep = FLECHA)

def rango(parametros, grafo_canciones):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit() or int(n) <= 0:
		print(NO_NUMERO.format(n))
		return
	if cancion not in grafo_canciones:
		print(NO_CANCION.format(cancion))
		return

	print(grafo_util.rango(grafo_canciones, cancion, int(n)))

# Calcula el pagerank de todos los usuarios y canciones del grafo de
# usuarios y devuelve una lista de tuplas de solo los vértices que
# eran canciones con (-1*pagerank, nombre cancion)
def _pagerank_canciones(grafo_usuarios):
	pr = grafo_util.pagerank(grafo_usuarios)
	lista = []
	for v in grafo_usuarios:
		if v[0] == ID_CANCION:
			lista.append((-1*pr[v], v[1]))

	return lista


def canciones_importantes(parametros, grafo_usuarios, top_canciones):
	n, _, _ = parametros.partition(" ")
	if not n.isdigit() or int(n) <= 0:
		print(NO_NUMERO.format(n))
		return
	n = int(n)
	es_heap = True

	cant_canciones = len(top_canciones[0]) + len(top_canciones[1])
	if cant_canciones == 0:
		top_canciones[1].extend(_pagerank_canciones(grafo_usuarios)) 
		es_heap = False
		cant_canciones = len(top_canciones[1])

	if n > cant_canciones:
		n = cant_canciones

	faltan_ordenar = n - len(top_canciones[0])

	if faltan_ordenar > 0:
		ordenados = _top_k(top_canciones[1], faltan_ordenar, es_heap = es_heap)
		top_canciones[0].extend([x[1] for x in ordenados])

	print(*top_canciones[0][:n], sep = SEPARADOR)

def clustering(parametros, grafo_canciones):
	if parametros == "":
		print(CLUST_FORMATO.format(float(grafo_util.clustering(grafo_canciones))))
		return

	if parametros not in grafo_canciones:
		print(NO_CANCION.format(parametros))
		return

	print(CLUST_FORMATO.format(float(grafo_util.clustering(grafo_canciones, vertice = parametros))))

# Procesa los parámetros de la función recomendacion
# Devuelve None si son inválidos, imprimiendo el mensaje de error
# Sino, devuelve una tupla con:
# * booleano pidio_canciones, true si se pidio canciones false si pidio usuarios
# * entero n, la cantidad de recomendaciones pedidas
# * lista_canciones, que contiene las canciones ingresadas ya como las tuplas 
# del grafo grafo_usuarios
def _procesar_param_recom(parametros, grafo_usuarios):
	que_recomendar, _, resto_parametros = parametros.partition(" ")
	pidio_canciones = que_recomendar == RECOM_CANCIONES 
	if not (pidio_canciones or que_recomendar == RECOM_USUARIOS):
		print(RECOM_ERROR)
		return None

	n, _, canciones = resto_parametros.partition(" ")
	if not n.isdigit() or int(n) <= 0:
		print(NO_NUMERO.format(n))
		return
	n = int(n)

	lista_canciones = canciones.split(SEPARADOR_CANCIONES)
	for i in range(len(lista_canciones)):
		c = lista_canciones[i]
		lista_canciones[i] = (ID_CANCION, c)
		if lista_canciones[i] not in grafo_usuarios:
			print(NO_CANCION.format(c))
			return None

	return pidio_canciones, n, lista_canciones

# Devuelve si el vértice pasado es del tipo pedido por el usuario
# (es decir, si es un usuario o una canción de acuerdo a lo que se pidio)
def _es_recom_valida(pidio_canciones, vertice):
	return (pidio_canciones and vertice[0] == ID_CANCION) or (not pidio_canciones and vertice[0] == ID_USUARIO)

def recomendacion(parametros, grafo_usuarios):
	datos = _procesar_param_recom(parametros, grafo_usuarios)
	if not datos:
		return
	pidio_canciones, n, lista_canciones = datos

	pagerank = grafo_util.pr_rand_walk(grafo_usuarios, lista_canciones)
	for c in lista_canciones:
		pagerank.pop(c, None)

	pagerank_lista = []
	for v, pr in pagerank.items():
		if _es_recom_valida(pidio_canciones, v):
			pagerank_lista.append((pr, v[1]))

	if n > len(pagerank_lista):
		n = len(pagerank_lista)

	print(*[x[1] for x in _top_k(pagerank_lista, n)], sep = SEPARADOR)