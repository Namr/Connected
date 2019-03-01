from PIL import Image
from PIL import ImageDraw
import random
import math

width = 1080
height = 1080
numNodes = 116
angleIncrement = 360.0 / 116.0
thickness = 50
nodePositions = []
colors = []
graphImage = Image.new('RGB', (width, height), color = 'black')
draw = ImageDraw.Draw(graphImage);

startAngle = 0
for node in range(numNodes):
	colors.append((random.randint(50,255), random.randint(50,255), random.randint(50,255)))
	draw.pieslice([0, 0, width, height], startAngle, startAngle + angleIncrement, fill = colors[node])
	nodePositions.append((math.cos((startAngle + startAngle + angleIncrement) / 2) * ((width - thickness * 2) / 2) + (width / 2), math.sin((startAngle + startAngle + angleIncrement) / 2) * ((height - thickness * 2) / 2) + (height / 2)))
	startAngle += angleIncrement	
draw.ellipse([thickness, thickness, width - thickness, height - thickness], fill = (0, 0, 0))
draw.point(nodePositions, fill = (0, 0, 0))

lines = [line.strip() for line in open("C:/Users/vrdem/Documents/GitHub/backuptry/Connectome/Release/assets/connect.edge")]

alreadyDone = []
for node in range(numNodes):
	for connectedNode in range(numNodes):
		if float(lines[(node * numNodes) + connectedNode]) >= 0.8 and node != connectedNode and (connectedNode, node) not in alreadyDone:
			alreadyDone.append((node, connectedNode))
			x1 = nodePositions[node][0] - (width / 2)
			y1 = nodePositions[node][1] - (height / 2)
			x2 = nodePositions[connectedNode][0] - (width / 2)
			y2 = nodePositions[connectedNode][1] - (height / 2)
			centerX = (x1 + x2) / 2
			centerY = (y1 + y2) / 2
			m1 = -x1/y1
			m2 = -x2/y2
			t = ( x1*m1 - x2*m2 - y1 + y2 ) / ( m1 - m2 )
			x0 = t
			y0 = (t-x2)*m2 + y2
			r = math.sqrt((x0 - x2)**2 + (y0-y2)**2)
									
			x0 += width / 2
			y0 += height / 2
			x1 += width / 2
			y1 += height / 2
			x2 += width / 2
			y2 += height / 2
			
			x = x0-r
			y = y0-r
			w = x0 + r
			h = y0 + r
	
			sA = math.degrees(math.atan2(y1-y0, x1-x0))
			eA = math.degrees(math.atan2(y2-y0, x2-x0))
			if eA < sA:
				eA += 360
			if sA + 180 < eA:
				sA, eA = eA, sA
				sA -= 360
			draw.arc([x, y, w, h], sA, eA, fill = colors[node])
			#draw.line([x1, y1, x2, y2], fill = 'red')
			
graphImage.save('C:/Users/vrdem/Documents/GitHub/backuptry/Connectome/Release/graph.png')