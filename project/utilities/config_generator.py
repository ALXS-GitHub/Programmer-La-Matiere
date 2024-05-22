import xml.etree.ElementTree as ET
import os
import random

def create_config_file(filename, x_basis, y_basis, n):
    root  = ET.Element("vs")
    visuals = ET.SubElement(root, "visuals")
    window = ET.SubElement(visuals, "window")
    window.set("size", "1280,720")
    window.set("backgroundColor", "#4D4DD0")
    render = ET.SubElement(visuals, "render")
    render.set("shadows", "off")
    render.set("grid", "on")

    world = ET.SubElement(root, "world")
    world.set("gridSize", f"{x_basis},{y_basis},{n}")

    camera = ET.SubElement(world, "camera")
    camera.set("target", "80,30,10")
    camera.set("thetaPhiDist", "0,30,200")
    camera.set("fov", "30")
    camera.set("near", "1.0")
    camera.set("far", "400.0")

    spotlight = ET.SubElement(world, "spotlight")
    spotlight.set("target", "80,30,0")
    spotlight.set("thetaPhiDist", "-35,40,300")
    spotlight.set("fov", "40")

    blockList = ET.SubElement(world, "blockList")
    blockList.set("color", "128,128,128")
    blockList.set("blockSize", "10,10,10")


    #création du tapis
    for i in range(0, x_basis):
        for j in range(0, y_basis):
            block = ET.SubElement(blockList, "block")
            block.set("position", str(i)+","+str(j)+",0")


    #ajout de n robots en couche supérieur ou égale à 1, disposés de manière aléatoire (attention à ne pas en mettre deux sur la même case)
    occupied_positions = []
    current_z = 1
    z_count = 0
    for i in range(0, n):
        x = x_basis - 1
        y = y_basis - 1
        z = current_z
        while (x, y, z) in occupied_positions:
            x = random.randint(0, x_basis -1)
            y = random.randint(0, y_basis -1)
            z = current_z
        z_count += 1
        occupied_positions.append((x, y, z))
        block = ET.SubElement(blockList, "block")
        block.set("position", str(x)+","+str(y)+","+str(z))
        block.set("color", "blue")
        block.set("leader", "1")
        if z_count == x_basis*y_basis:
            current_z += 1
            z_count = 0


    
    tree = ET.ElementTree(root)
    tree.write(filename)



if __name__ == "__main__":
    create_config_file("config.xml", 20, 20, 80)