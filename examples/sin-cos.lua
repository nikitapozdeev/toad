function draw()
    cls()
    
    -- Draw center line
    for x = 0, 127 do
        putp(x, 64, 5) -- gray center line
    end

    -- Draw animated sine wave using sin()
    local t = time()
    for x = 0, 127 do
        local angle = x / 128.0 + t * 0.5 -- slow animation
        putp(x, 64 + sin(angle) * 20, 12) -- blue sine wave
        putp(x, 64 + cos(angle) * 20, 8) -- red cosine wave
    end
end