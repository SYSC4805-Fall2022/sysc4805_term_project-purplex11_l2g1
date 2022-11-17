import pygame
import pygame_menu
from functools import partial
import subprocess
import os

WIDTH = 900
WIN = pygame.display.set_mode((WIDTH,WIDTH)) 
RED = (255,0,0)
GREEN = (0,255,0)
BLUE = (0,0,255)
YELLOW = (255,255,0)
WHITE = (255,255,255)
BLACK = (0,0,0)
PURPLE = (128,0,128)
ORANGE = (255,165,0)
GREY = (128,128,128)
TURQUOISE = (64,224,208)

ROWS = 150

VERTICAL = []
HORIZONTAL = []

NorthBorderRow = 100
EastBorderCol = 110
obstacles = [(20,20,5,5), (30,30,5,5), (60,30,2,5)]

ARR_SIZE = 150

def run():
    res = [[]]
    for i in range(ARR_SIZE):
        res.append([])
        for y in range (ARR_SIZE):
            res[i].append(0)

    for i in range(ARR_SIZE):
        for y in range(ARR_SIZE):
            if i > NorthBorderRow:
                res[i][y] = 0
            elif y > EastBorderCol:
                res[i][y] = 0
            else:
                if i == NorthBorderRow:
                    res[i][y] = 1
                
                elif y == EastBorderCol:
                    res[i][y] = 1

                else:
                    for val in obstacles:
                        if val[0] <= i and (val[0] + val[2]) > i and val[1] <=y and (val[1] + val[3]) > y:
                            res[i][y] = 3

                    if res[i][y] != 3:
                        res[i][y] = 2

    return res

def printToFile(res):
    with open("generated_map.txt", 'w') as f:
        f.write(f'uint8_t tmp[{ARR_SIZE}][{ARR_SIZE}] = ' + '{\n')
        firstCol = False
        for row in res:
            if len(row) > 0:
                firstCol = True
                f.write('   {')
                for col in row:
                    if not firstCol:
                        f.write(', ')
                    f.write(str(col))
                    firstCol = False

                f.write('},\n')
        f.write('};')

def update_map():
    res = run()
    printToFile(res)
    lines = []
    in_map = False
    with open('pathFindingTest.cpp', 'r') as f:
        for line in f.readlines():
            if 'tmp' in line and 'uint8_t ' in line and ' = {' in line:
                in_map = True
            
            if in_map:
                if '};' in line:
                    in_map = False
                    lines.append('\n')
                continue
            if  not in_map:
                lines.append(line)

    start_index = 0
    for index, line in enumerate(lines):
        if 'Start Writing Flag' in line:
            start_index = index + 1

    map_lines = []
    with open('generated_map.txt', 'r') as f:
        map_lines = f.readlines()
    
    for line in map_lines:
        lines.insert(start_index, line)
        start_index += 1

    with open('pathFindingTest.cpp','w') as f:
        f.writelines(lines)
    
    os.remove('generated_map.txt')



def run_cpp():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    subprocess.run(['g++', '../libraries_v2/PathFinding/pathFinding.cpp', 'pathFindingTest.cpp', '../libraries_v2/PathFinding/pathFinding.h', '-o', 'pathFindingTest.exe'])
    var = subprocess.run(['./pathFindingTest.exe'],
                            stdout=subprocess.PIPE, 
                            universal_newlines=True)
    global HORIZONTAL
    global VERTICAL
    HORIZONTAL = [int(x) for x in var.stdout.split('\n')[0].strip().split(' ')]
    VERTICAL = [int(x) for x in var.stdout.split('\n')[1].strip().split(' ')]
    for index, val in enumerate(HORIZONTAL):
        if val == 0 and index > 0:
            HORIZONTAL = HORIZONTAL[:index]
            break

    for index, val in enumerate(VERTICAL):
        if val == 0 and index > 0:
            VERTICAL = VERTICAL[:index]
            break
    os.remove('./pathFindingTest.exe')

class Node:
    def __init__(self, row, col, width, color):   
        self.row = row                           
        self.col = col
        self.x = row*width
        self.y = col*width
        self.color = color                  
        self.width = width
    #get position of the node
    def get_pos(self):                                  
        return self.row,self.col
    # return true if node is closed
    def is_left (self):                                
        return self.color == RED
    # return true if node is open
    def is_passed(self):                                  
        return self.color == GREEN
    # returns true is node is a wall
    def is_obstacle(self):                                                    
        return self.color == BLACK
    # draw the node on the window
    def draw(self,win):                                 
        pygame.draw.rect(win,self.color,(self.x,self.y,self.width,self.width))

#add the grid lines to the display
def draw_grid(win,rows,width):
    gap = width // rows
    for i in range(rows):
        pygame.draw.line(win,GREY,(0,i*gap),(width,i*gap))
        for j in range(rows):
            pygame.draw.line(win,GREY,(j*gap,0),(j*gap,width))
#draw on the window
def draw(win,grid,rows,width):
    win.fill(WHITE)
    for row in grid:
        for node in row:
            node.draw(win)
    draw_grid(win,rows,width)
    pygame.display.update()

# make a 2d array of nodes 
def make_grid(rows,width):
    global HORIZONTAL
    global VERTICAL
    full_map = run()
    grid = []
    gap = width // rows
    for i in range(rows):
        grid.append([])
        for j in range(rows):
            color = None
            for h in HORIZONTAL:
                if i in range (h, h+8):
                    color = GREEN
            for v in VERTICAL:
                if j in range (v, v+8):
                    color = GREEN
            if full_map[i][j] == 3:
                color = BLACK

            if i > NorthBorderRow:
                color=WHITE
            
            if j > EastBorderCol:
                color = WHITE

            if color == None:
                color = RED
                
            node = Node(i,j,gap, color)
            grid[i].append(node)
    return grid

# main function to run the program
def main(win,width):
    # set caption of the window
    pygame.display.set_caption("Path Finding Algorithm")
    # initialize variables
    ROWS = 150
    grid = make_grid(ROWS,width)
    draw(win,grid,ROWS,width)
    clock = pygame.time.Clock()
    # main loop to run program
    while True:
        clock.tick(120)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()

#display the main menu
def main_menu():
    pygame.init()
    pygame.display.set_caption("Menu")
    surface = pygame.display.set_mode((WIDTH,WIDTH))
    menu = pygame_menu.Menu(300,400,"Welcome",theme=pygame_menu.themes.THEME_BLUE)
    menu.add_button("Enter",partial(main,WIN,WIDTH))
    menu.mainloop(surface)

if __name__ == "__main__":
    update_map()
    run_cpp()
    main_menu()