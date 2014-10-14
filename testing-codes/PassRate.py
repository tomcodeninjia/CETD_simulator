#!/usr/bin/python

class Node(object):
	def __init__(self,val,p=0):
		self.data = val
		self.next = p

class LinkList(object):
	def __init__(self):
		self.head = 0

	def initlist(self,data):
		self.head = Node(data[0])
		p = self.head

		for i in data[1:]:
			node = Node(i)
			p.next = node
			p = p.next


l = LinkList()
l.initlist([(1,1), (2,2), (3,3)])

