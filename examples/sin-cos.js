// JavaScript version of sin-cos example
let t = 0;

function draw() {
    cls();
    const t = time();

    // Draw center line
    for (let x = 0; x < 128; x++) {
        putp(x, 64, 5) // gray center line
    }

    // Draw sine and cosine waves
    for (let x = 0; x < 128; x++) {
        const angle = x / 128.0 + t * 0.5
        putp(x, 64 + sin(angle) * 20, 12) // blue sine wave
        putp(x, 64 + cos(angle) * 20, 8) // red cosine wave
    }
}