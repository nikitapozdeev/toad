x = 0
y = 0
c = 0

function update()
    -- Game logic
    x = x + 1;
    y = x / 128;
    c = c + 1
end

function draw()
    -- Rendering
    putp(x % 128, y % 128, c % 16)
end