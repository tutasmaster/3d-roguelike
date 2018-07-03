#pragma once

#include "Map.h"

void GeneratePerlinNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GeneratePerlinFBMNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getFbm(p,32.f, TCOD_NOISE_PERLIN) * 15) + 15;
			map->SetTileAt(0, 0, 0, TileManager::wall);
			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GeneratePerlinTurbulenceNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getTurbulence(p, 32.f, TCOD_NOISE_PERLIN) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GeneratePerlinRigidNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)((1-(-abs(noise.get(p, TCOD_NOISE_PERLIN))+1)) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateSimplexNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.get(p, TCOD_NOISE_SIMPLEX) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateSimplexFBMNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getFbm(p,32.f, TCOD_NOISE_SIMPLEX) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateSimplexTurbulenceNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getTurbulence(p, 32.f, TCOD_NOISE_SIMPLEX) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateWaveletNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.get(p, TCOD_NOISE_WAVELET) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateWaveletFBMNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getFbm(p, 32.f, TCOD_NOISE_WAVELET) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}

void GenerateWaveletTurbulenceNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float p[2] = { i*0.1, j*0.1 };
			int value = (int)(noise.getTurbulence(p, 32.f, TCOD_NOISE_WAVELET) * 15) + 15;

			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}