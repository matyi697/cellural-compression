import numpy as np

def read_binary_file(filename):
    with open(filename, "rb") as f:
        data = f.read()
    return np.unpackbits(np.frombuffer(data, dtype=np.uint8))

def apply_cellular_automaton(data, steps=1):
    length = len(data)
    for _ in range(steps):
        new_data = np.zeros_like(data)
        for i in range(length):
            left = data[i - 1] if i > 0 else data[-1]
            right = data[i + 1] if i < length - 1 else data[0]
            # Egyszerűsített szabály: az XOR a szomszédok állapotából
            new_data[i] = left ^ right
        data = new_data
    return data

def write_binary_file(filename, data):
    packed_data = np.packbits(data)
    with open(filename, "wb") as f:
        f.write(packed_data)

# Main program
input_filename = "input.bin"
output_filename = "output.bin"

# Read the binary file
binary_data = read_binary_file(input_filename)

# Apply the cellular automaton
modified_data = apply_cellular_automaton(binary_data, steps=1)

# Write the modified data to a new file
write_binary_file(output_filename, modified_data)

print(f"Sejtautomata alkalmazása megtörtént, az eredmény az {output_filename} fájlban található.")
