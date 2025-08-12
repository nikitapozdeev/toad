function draw() {
    cls();
    
    const t = time();
    const center_x = 64;
    const center_y = 64;
    const radius = 30;
    
    // Slow rotation for better visibility
    const angle = t * 0.1; // very slow
    
    // Main rotating point
    const x = center_x + cos(angle) * radius;
    const y = center_y + sin(angle) * radius;
    
    putp(x, y, 8); // red point
    
    // Draw center
    putp(center_x, center_y, 7); // white point in center
    
    // Draw circle for reference
    for (let i = 0; i <= 15; i++) {
        const ref_angle = i / 16;
        const ref_x = center_x + cos(ref_angle) * radius;
        const ref_y = center_y + sin(ref_angle) * radius;
        putp(ref_x, ref_y, 5); // gray circle points
    }
    
    // Show direction: arrow from center to point
    const steps = 10;
    for (let i = 1; i <= steps; i++) {
        const progress = i / steps;
        const arrow_x = center_x + cos(angle) * radius * progress;
        const arrow_y = center_y + sin(angle) * radius * progress;
        putp(arrow_x, arrow_y, 12); // blue arrow
    }
    
    // Mark initial position (angle = 0)
    const start_x = center_x + cos(0) * radius;
    const start_y = center_y + sin(0) * radius;
    putp(start_x, start_y, 11); // green start mark
}