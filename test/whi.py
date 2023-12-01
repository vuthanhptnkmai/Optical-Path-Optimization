import random
import math

def generate_ray():
    # Assuming you want random directions
    dx = random.uniform(-0.75, 0.75)
    dy = random.uniform(-2, -2.14)
    dz = random.uniform(-0.75, 0.75)

    # Normalize the direction vector
    magnitude = math.sqrt(dx*dx + dy*dy + dz*dz)
    dx /= magnitude
    dy /= magnitude
    dz /= magnitude

    wavelength = random.randint(450, 500)
    intensity = 1

    return 0, 2, 0, dx, dy, dz, wavelength, intensity

def main():
    with open("ray_input.dat", "w") as f:
        for _ in range(30):
            ray = generate_ray()
            f.write(" ".join(map(str, ray)) + "\n")

if __name__ == "__main__":
    main()
