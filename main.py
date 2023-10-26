import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.cluster import KMeans

def rgb_to_hex(rgb_color):
    return "#{:02x}{:02x}{:02x}".format(int(rgb_color[0]), int(rgb_color[1]), int(rgb_color[2])

def visualize_color_palette(image, colors):
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(6, 8))
    
    # Display the original image
    ax1.imshow(image)
    ax1.set_title("Original Image")
    ax1.axis('off')
    
    # Display the color palette
    color_palette = [rgb_to_hex(color) for color in colors]
    sns.palplot(color_palette)
    ax2.set_title("Color Palette")
    ax2.axis('off')
    
    # Display the most dominant color
    dominant_color = colors[0]
    dominant_color_hex = rgb_to_hex(dominant_color)
    ax3.imshow([dominant_color])
    ax3.set_title(f"Dominant Color ({dominant_color_hex})")
    ax3.axis('off')
    
    plt.tight_layout()
    plt.show()

def calculate_color_percentages(labels):
    total_colors = len(labels)
    unique_labels, counts = np.unique(labels, return_counts=True)
    percentages = counts / total_colors
    return unique_labels, percentages

def main(image_path):
    try:
        img_org = cv2.imread(image_path)
        img_org = cv2.cvtColor(img_org, cv2.COLOR_BGR2RGB)
        img = cv2.resize(img_org, (250, 250))
        r, g, b = cv2.split(img)
        r = r.flatten()
        g = g.flatten()
        b = b.flatten()
        img = img.reshape(img.shape[0] * img.shape[1], 3)

        print("Starting clustering...")
        palette = KMeans(n_clusters=10, init='random',
            n_init=10, max_iter=300,
            tol=1e-04, random_state=0)
        palette.fit(img)
        print("Clustering done.")

        colors = palette.cluster_centers_.astype(int)
        hex_codes = [rgb_to_hex(color) for color in colors]
        labels = palette.labels_

        print(f'The color palette is:\n\tHEX: {hex_codes}\n\tRGB: {colors}')

        visualize_color_palette(img_org, colors)

        unique_labels, percentages = calculate_color_percentages(labels)
        for label, percentage in zip(unique_labels, percentages):
            hex_color = rgb_to_hex(colors[label])
            print(f"Color Label {label} ({hex_color}): {percentage*100:.2f}%")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <image_path>")
    else:
        main(sys.argv[1])
