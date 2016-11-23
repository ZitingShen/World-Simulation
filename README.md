# World-Simulation

Ziting Shen & Qin Yang

Control:

View: 
	'v' -- Default View (use up/dwon to zoom in/out at the island)
	't' -- Trailing View (use up/dwon to zoom in/out at the flock)
	'g' -- Side View (use up/dwon to zoom in/out at the flock)
	'f' -- First Person View (use mouse to control where to look at the the headlight)
	'o' -- Following View with centre at the island
	'l' -- Environmental Map View (Slowly rotating around the ball)
	P.S. Headlight is only steerable in First Person View; in other mod, it will follow designed boid and project at the direction of its velocity

Teleport:
	'1' -- Teleport the flock to a place near the island (best with Side View -- 'G')
	'2' -- Teleport the flock to a place far from the island (best with Following View -- 'O')
	'3' -- Teleport the flock to a place high above the island (bewst with First Person View -- 'F')

Island:
	1. Procedually generated with three triangulation level depending on the proximity of view
	2. Two textures, with snow at the top of the mountain with real-time generated snow-line
	3. Trees (mesh downloaded from the web) randomly populated the surface of island

Sun/Lighting:
	1. Smooth nocturnal transition as sun tours around the sky
	2. Steerable Headlight (spotlight) on one boid with natural attenuation
	3. Ambient light at night

Enviornment Mapping:
	1. Still appears incongruous when some faces meet; not sure if it its the skybox's fault


Caveats:
	1. After pressing 1, 2, 3, you also need to switch to different views to see the boids. Otherwise they might be too small in the default view or not visible in the default view at all.

	2. We tried our best to control the boid headlight with the mouse. While it works well for the environment mapped sphere, it does not work exactly correct for the normal phong shader. Actually, it also used to work well in the phong shader, until we changed other parts of code which broke this part. We spent the whole night trying to fix it but still failed. Now the spotlight can be roughly controled by the mouse, but the spotlight is not centered at the cursor location.

	3. Loading our nice textures takes some time. Running the program on the mac might slow down the frame rate. We recommend running our program in 231.