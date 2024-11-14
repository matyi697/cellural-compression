import heapq
import os
import sys
from collections import defaultdict, Counter

# Huffman Tree node
class Node:
    def __init__(self, freq, symbol=None, left=None, right=None):
        self.freq = freq
        self.symbol = symbol
        self.left = left
        self.right = right

    def __lt__(self, other):
        return self.freq < other.freq

class HuffmanCoding:
    def __init__(self):
        self.codes = {}
        self.reverse_codes = {}

    def build_frequency_table(self, data):
        return Counter(data)

    def build_huffman_tree(self, frequency_table):
        heap = [Node(freq, symbol) for symbol, freq in frequency_table.items()]
        heapq.heapify(heap)

        while len(heap) > 1:
            left = heapq.heappop(heap)
            right = heapq.heappop(heap)
            merged = Node(left.freq + right.freq, left=left, right=right)
            heapq.heappush(heap, merged)

        return heap[0]

    def build_codes(self, node, current_code=""):
        if node is None:
            return

        if node.symbol is not None:
            self.codes[node.symbol] = current_code
            self.reverse_codes[current_code] = node.symbol
            return

        self.build_codes(node.left, current_code + "0")
        self.build_codes(node.right, current_code + "1")

    def get_encoded_data(self, data):
        return ''.join([self.codes[symbol] for symbol in data])

    def pad_encoded_data(self, encoded_data):
        extra_padding = 8 - len(encoded_data) % 8
        padded_info = "{0:08b}".format(extra_padding)
        padded_data = encoded_data + '0' * extra_padding
        return padded_info + padded_data

    def remove_padding(self, padded_encoded_data):
        padded_info = padded_encoded_data[:8]
        extra_padding = int(padded_info, 2)
        padded_encoded_data = padded_encoded_data[8:]
        encoded_data = padded_encoded_data[:-extra_padding]
        return encoded_data

    def compress(self, input_file, output_file):
        with open(input_file, 'rb') as f:
            data = f.read()

        frequency_table = self.build_frequency_table(data)
        huffman_tree = self.build_huffman_tree(frequency_table)
        self.build_codes(huffman_tree)

        encoded_data = self.get_encoded_data(data)
        padded_encoded_data = self.pad_encoded_data(encoded_data)

        b = bytearray()
        for i in range(0, len(padded_encoded_data), 8):
            byte = padded_encoded_data[i:i+8]
            b.append(int(byte, 2))

        with open(output_file, 'wb') as f:
            f.write(bytes(b))

        return output_file

    def decode(self, encoded_data):
        current_code = ""
        decoded_data = bytearray()

        for bit in encoded_data:
            current_code += bit
            if current_code in self.reverse_codes:
                symbol = self.reverse_codes[current_code]
                decoded_data.append(symbol)
                current_code = ""

        return bytes(decoded_data)

    def decompress(self, input_file, output_file):
        with open(input_file, 'rb') as f:
            bit_string = ""
            byte = f.read(1)
            while len(byte) > 0:
                byte = ord(byte)
                bits = bin(byte)[2:].rjust(8, '0')
                bit_string += bits
                byte = f.read(1)

        encoded_data = self.remove_padding(bit_string)

        decompressed_data = self.decode(encoded_data)

        with open(output_file, 'wb') as f:
            f.write(decompressed_data)

        return output_file

# Parancssori feldolgozás
def main():
    if len(sys.argv) != 4:
        print("Használat: python huffman.py -c/-d input_file output_file")
        sys.exit(1)

    flag = sys.argv[1]
    input_file = sys.argv[2]
    output_file = sys.argv[3]

    huffman = HuffmanCoding()

    if flag == '-c':
        huffman.compress(input_file, output_file)
    elif flag == '-d':
        huffman.decompress(input_file, output_file)
    else:
        print("Ismeretlen opció. Használat: -c tömörítéshez, -d visszafejtéshez.")
        sys.exit(1)

if __name__ == "__main__":
    main()
