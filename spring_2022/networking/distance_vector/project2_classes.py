import numpy as np
from tkinter import *
from tkinter import ttk

# Node class for links
class Node: 
    """Node class containing links to other nodes
    Name: The node owing this object. 
    Connections: A dictonairy where each key maps a cost from the node to it. 
    connections => Node A{
        Node B: Costs C
    }
    """
    def __init__(self, linkArray):
        self.name = linkArray[0]
        # Create the inital dictionary  
        self.connections = {
            linkArray[0]: 0,
            linkArray[1]: linkArray[2]
            }
        # Create the neighbors 
        self.neighbors = []
        # Get the required next hop 
        self.next = {
            linkArray[0]: 0,
            linkArray[1]: linkArray[1]
        }

    def add_link(self, inputs):
        # function adds the new links to the object, cost and next hop 
        connection = inputs[0]
        cost = inputs[1]
        if connection not in self.connections:
            self.connections[connection] = cost
            if cost == np.inf:
                self.next[connection] = cost
            else:
                self.next[connection] = connection
        else:
            raise Exception("Cannot have two links between the same nodes.")
    
    def get_connections(self):
        return self.connections

    def update_connections(self, list):
        self.connections[list[0]] = list[1] 
        self.next[list[0]] = np.inf
    
    def get_dv(self):
        tableData = []
        nodes = sorted(self.connections.keys())
        # Creates a combined tripule of shape (destination, cost, next_hop)
        for key in nodes:
             tableData.append((key, self.connections[key], self.next[key]))

        return tableData

    def get_costs(self, node = None):
        if node == None:
            return list(self.connections.values())
        else:
            return list(self.connections[node])
    
    def get_keys(self): 
        return self.connections.keys()
    
    def set_neighbors(self):
        self.neighbors = list(self.get_keys())[1:]
    
    def get_neighbors(self):
        return self.neighbors
    
    def update_from_neighbor(self, sender, dv):
        # Need connecting values from self 
        # Finds the required cost ie 1->2 cost if sender is 2 
        costToSender = self.connections[sender]
        # Check connection checks if the node is stable after this round of updating
        checkConnection = self.get_costs()
        for key, cost in dv.items():   
            # If current key is the sender or itself skip. 
            # This is becuase from the sender there is no better way to reach it. 
            # Cost to reach itself is 0 
            if key == sender or key == self.name:
                continue
            else:
                # Get the current cost before update
                currentCost = self.connections[key]
                # Calculate the new estimated cost, if less 
                # then update the costs. 
                newPossibleCost = costToSender + cost
                if currentCost > newPossibleCost:
                    self.connections[key] = newPossibleCost
                    self.next[key] = sender
        # If after all calculations, nothing got updated notify dvr function. 
        if checkConnection == self.get_costs():
            return True
        else:
            return False

# Gui Table Class to draw dv tables in each window
class Table:
    def __init__(self, window, links):
        self.frame = Frame(window)
        self.frame.pack()

        self.table = ttk.Treeview(self.frame)
        self.table['columns'] = ('destination', 'cost', 'nextHop')

        # Setup columns
        self.table.column("#0", width=0, stretch=NO)
        self.table.column("destination", anchor=CENTER, width=100)
        self.table.column("cost", anchor=CENTER, width=100)
        self.table.column("nextHop", anchor=CENTER, width=100)

        #Set headings
        self.table.heading("#0", text="", anchor=CENTER)
        self.table.heading("destination", text="Destination", anchor=CENTER)
        self.table.heading("cost", text="Estimated Cost", anchor=CENTER)
        self.table.heading("nextHop", text="Next Hop", anchor=CENTER)

        # Set initial table information
        for i, link in enumerate(links): 
            self.table.insert(parent='', index='end', iid=i, text="", values=link)

        self.table.pack()
    
    def update_items(self, links):
        # Update the table based off new information
        for i, link in enumerate(links): 
            self.table.item(i, values=link)
        self.table.pack()
        return