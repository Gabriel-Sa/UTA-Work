# Gabriel de Sa 1001676832 Lab 2 -- Computer Networks
#%%
import numpy as np
from tkinter import *
from tkinter import ttk
from tkinter import messagebox
from project2_classes import *
from threading import Thread
import time

global should_step, linkMap, time_label, time_label_obj, start_button, step_button
should_step = True

def dvr_stepwise(linkMap):
    # Send initial Updates to each neighbor
    stable, checkStability = {}, {}
    global should_step, time_label, time_label_obj
    for key, node in linkMap.items():
        # At each while loop wait for the user to click next step
        while(should_step == False): 
            continue
        stable[key] = [] # Create stability check
        checkStability[key] = [] # Create list to check stable against
        neighbors = node.get_neighbors() # Select neighbors from current Node (i.e. 1 has neighbors 2 and 5)
        dv = node.get_connections() # Get the current connections 
        for neighbor in neighbors: # For each neighbor of i update them about i's status
            checkStability[key].append(True)
            stable[key].append(False)
            linkMap[neighbor].update_from_neighbor(node.name, dv) # Do the updating
        should_step = False
        tables[key].update_items(node.get_dv())
    # Continue upgrading until stable. 
    should_step = False
    while stable != checkStability: # Make sure there aren't any new changes
        # Update neighbors
        for key, node in linkMap.items(): 
            while(should_step == False):
                continue
            neighbors = node.get_neighbors() # Get all neighbors of the current key
            dv = node.get_connections() # Get current node connections dv 
            for i, neighbor in enumerate(neighbors): # Update each neighbor of node i 
                stable[key][i] = linkMap[neighbor].update_from_neighbor(node.name, dv) 
            tables[key].update_items(node.get_dv())
            should_step = False
    time_label.set("Stable State Reached!") # set the time label to indicate a stable state
    time_label_obj.pack(pady=10)
    return 

def dvr(linkMap):
    # Send initial Updates to each neighbor
    start = time.time()
    print("After First Round of Updating:")
    stable, checkStability = {}, {}
    for key, node in linkMap.items():
        stable[key] = [] # Create stability check
        checkStability[key] = [] # Create list to check stable against
        neighbors = node.get_neighbors() # Select neighbors from current Node (i.e. 1 has neighbors 2 and 5)
        dv = node.get_connections() # Get the current connections 
        for neighbor in neighbors: # For each neighbor of i update them about i's status
            checkStability[key].append(True)
            stable[key].append(False)
            linkMap[neighbor].update_from_neighbor(node.name, dv) # Do the updating
        print(node.get_connections())
        tables[key].update_items(node.get_dv())
    # Continue upgrading until stable. 
    counter = 1 # In order to keep track number of loops required to reach stability
    while stable != checkStability: # Make sure there aren't any new changes
        print("Round {}".format(counter)) 
        # Update neighbors
        for key, node in linkMap.items(): 
            neighbors = node.get_neighbors() # Get all neighbors of the current key
            dv = node.get_connections() # Get current node connections dv 
            for i, neighbor in enumerate(neighbors): # Update each neighbor of node i 
                stable[key][i] = linkMap[neighbor].update_from_neighbor(node.name, dv) 
            print(node.get_connections())
            tables[key].update_items(node.get_dv())
        counter = counter + 1
    end = time.time()
    time_label.set("Took {:.4f} seconds.".format(end-start))
    time_label_obj.pack(pady=10)
    return 

def main(file):
    graphFile = file
    link_map = {}
    # Create inital routing tables 
    with open(graphFile) as file:
        for line in file.readlines():
            disection = list(map(lambda x : int(x), line.split()))
            # Add first column of connections
            if disection[0] not in link_map:  # Creates object if does not exist in objectmap
                link_map[disection[0]] = Node(disection)
            else:
                link_map[disection[0]].add_link(disection[1:]) # if the object does exist then add the link data
            # Add second column of connections, basically making fully connected map. 
            if disection[1] not in link_map: # Do the same but from the second column
                link_map[disection[1]] = Node([disection[1], disection[0], disection[2]])
            else:
                link_map[disection[1]].add_link([disection[0], disection[2]])
    # Add missing nodes
    print("Initial Connections:")
    keys = set(link_map.keys()) #Print out the inital connections
    for node in link_map.values(): 
        node.set_neighbors() # Set the neighbors before any changes happen
        missing_links = list(keys - node.get_keys()) # Create a list of links that aren't connected
        for n in missing_links: 
            node.add_link([n, np.inf]) # Add inf to every link that isn't directly connected to node
        print(node.get_connections())
    # return the link_map
    return link_map 

def createBase(file):
    # Once the user loads the file, then begin 
    global start_button, step_button, time_label, time_label_obj, linkMap
    start_button = Button(root, text="Route Completely", command=lambda: dvr(linkMap))  # Create the contiuous button
    step_button = Button(root, text="Stepwise Routing", command=buttonClicked) # create the stepwise button 
    time_label = StringVar() # Create the time label 
    time_label_obj = Label(root, textvariable=time_label) # create the time object 
    start_button.pack(pady=10) # Add the start button to the window
    step_button.pack(pady=10) # add the step button to the window 
    linkMap = main(file) # Create the linkMap
    # Need to show initial table setup
    for name, node in linkMap.items():
        windows[name] = Toplevel()  # Create the new window for each node
        windows[name].title("Table for Node {}".format(name)) # Add a title to the window 
        windows[name].geometry("305x210") 
        linkData = node.get_dv() # Get the dv values for that node
        tables[name] = Table(windows[name], linkData) # Create the table inside the window

def nextStep():
    global should_step 
    should_step = True

def changeLink(links):
    links = list(map(lambda x: int(x), links))
    node = links[0]
    # Need to update table. 
    linkMap[node].update_connections(links[1:])
    tables[node].update_items(linkMap[node].get_dv())

def buttonClicked():
    global start_button, should_step, linkMap 
    start_button.destroy() # Destroy the start and step button and replace it with a next step button 
    step_button.destroy()
    root.geometry("330x330")
    next_step_btn = Button(root, text="Next Step", command=nextStep)
    next_step_btn.pack(pady=10)
    change_btn = Button(root, text="Change Links", command=lambda: changeLink([node_values.get(), destination_values.get(), cost_values.get()]))
    destination_values = StringVar()
    destination_entry = ttk.Entry(root, textvariable = destination_values, justify = CENTER)
    node_values = StringVar()
    node_entry = ttk.Entry(root, textvariable = node_values, justify = CENTER)
    cost_values = StringVar()
    cost_entry = ttk.Entry(root, textvariable = cost_values, justify = CENTER)
    # Render the entry box 
    node_entry.pack(pady=10)
    destination_entry.pack(pady=10)
    cost_entry.pack(pady=10)
    change_btn.pack(pady=5)
    t = Thread(target=dvr_stepwise, args=(linkMap,)) # Create a new thread that does the stepwise function
    t.start()

def loadFile(file):
    try:
        open(file)
    except:
        messagebox.showerror(title="Couldn't Find File", message="Could not find {}, please try again.".format(file))
    else:
        file_entry.destroy()
        load_button.destroy()
        createBase(file)

# Initiate Variables to contain GUI
windows = {}
tables = {}
# Create root window
root = Tk()
# Set window size
root.geometry("305x210")
# Title the window
root.title("Project 2 - DVR")
# Create a string indicating what the window represents
label = StringVar()
label.set("Project 2 - Distance Vector Routing")
root_label = Label(root, textvariable=label)
root_label.pack(pady=10)
# Create an entry box so the user can input the file location
file_name = StringVar()
file_entry = ttk.Entry(root, textvariable = file_name, justify = CENTER)
# Get user focus on the entry box
file_entry.focus_force()
# Render the entry box 
file_entry.pack(pady=10)
# Create button to load user entry 
load_button = Button(root, text="Load File", command=lambda: loadFile(file_name.get()))
load_button.pack(pady=10)

root.mainloop()

# %%
