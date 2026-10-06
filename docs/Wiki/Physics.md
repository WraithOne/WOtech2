# Physics

box3d is wrapped by `PhysicsWorld`. Components store body type, mass, collider shape, and velocities. The wrapper creates bodies, steps the simulation, and writes transforms back. Lua can set velocity and apply forces through the wrapper. Tests do not include box3d types.
