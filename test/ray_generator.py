import xml.etree.ElementTree as ET
import random

# Create the root element
root = ET.Element("rays")

# Define the grid size and spacing
grid_size = 20
spacing = 1.0

# For each point in the grid, create a ray with a random direction
for i in range(grid_size):
    for j in range(grid_size):
        ray = ET.SubElement(root, "ray")

        position = ET.SubElement(ray, "position")
        position.set("x", str(i * spacing))
        position.set("y", str(j * spacing))
        position.set("z", "0.0")  # All rays start from z=0 for simplicity

        direction = ET.SubElement(ray, "direction")
        direction.set("dx", str(random.uniform(-1, 1)))
        direction.set("dy", str(random.uniform(-1, 1)))
        direction.set("dz", str(random.uniform(-1, 1)))

        wavelength = ET.SubElement(ray, "wavelength")
        wavelength.set("value", str(random.randint(400, 700)))  # Random wavelength between 400 and 700 nm

        intensity = ET.SubElement(ray, "intensity")
        intensity.set("value", "1.0")  # All rays have intensity 1.0 for simplicity

# Write the XML to a file
tree = ET.ElementTree(root)
with open("many_rays.xml", "wb") as f:
    tree.write(f)

print(f"Generated {grid_size * grid_size} rays in 'many_rays.xml'")
