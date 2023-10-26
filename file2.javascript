<!DOCTYPE html>
<html>
<head>
    <title>Color Palette Extraction</title>
</head>
<body>
    <input type="file" id="imageInput" accept="image/*">
    <div id="colorPalette"></div>
    <canvas id="imageCanvas" style="display:none;"></canvas>

    <script>
        const imageInput = document.getElementById('imageInput');
        const colorPalette = document.getElementById('colorPalette');
        const canvas = document.getElementById('imageCanvas');
        const ctx = canvas.getContext('2d');

        imageInput.addEventListener('change', handleImageUpload);

        function handleImageUpload(event) {
            const file = event.target.files[0];
            if (file) {
                const reader = new FileReader();

                reader.onload = function(e) {
                    const image = new Image();
                    image.src = e.target.result;

                    image.onload = function() {
                        canvas.width = image.width;
                        canvas.height = image.height;
                        ctx.drawImage(image, 0, 0, canvas.width, canvas.height);

                        extractColorPalette();
                    };
                };

                reader.readAsDataURL(file);
            }
        }

        function rgbToHex(color) {
            return '#' + ((1 << 24) | (color[0] << 16) | (color[1] << 8) | color[2]).toString(16).slice(1).toUpperCase();
        }

        function extractColorPalette() {
            const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
            const data = imageData.data;
            const colorCounts = {};

            for (let i = 0; i < data.length; i += 4) {
                const color = [data[i], data[i + 1], data[i + 2]];
                const hexColor = rgbToHex(color);

                if (!colorCounts[hexColor]) {
                    colorCounts[hexColor] = 0;
                }

                colorCounts[hexColor]++;
            }

            const sortedColors = Object.keys(colorCounts).sort((a, b) => colorCounts[b] - colorCounts[a]);
            displayColorPalette(sortedColors);
        }

        function displayColorPalette(colors) {
            const colorPaletteHTML = colors.map(color => {
                return `
                    <div style="background-color:${color}; width: 50px; height: 50px;"></div>
                    <span>${color}</span>
                `;
            }).join('');

            colorPalette.innerHTML = colorPaletteHTML;
        }
    </script>
</body>
</html>
