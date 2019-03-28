#pragma once

#include "Map.h"
#include <algorithm>

void GeneratePerlinNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	float r = tcodRand->getFloat(0,1);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			
			float pBiome[2] = { (i * 0.01) + r * 100, (j * 0.01) + r * 100 };
			float pBiome2[2] = { (i * 0.025) + r * 1000, (j * 0.025) + r * 1000 };
			float biomeValue = noise.get(pBiome, TCOD_NOISE_PERLIN);
			float riverValue = (-abs(noise.get(pBiome2, TCOD_NOISE_PERLIN) * 2 - 1 )+1);

			float p[2];
			int value;

			if(biomeValue > 0.1){
				p[0] = i * 0.1f;
				p[1] = j * 0.1f;
				value = (int)(noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15;
			}
			else {
				p[0] = i * 0.05f;
				p[1] = j * 0.05f;
				value = (int)(noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22;
			}

				p[0] = i * 0.05f;
				p[1] = j * 0.05f;
				float value2 = ((noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22);
				p[0] = (i + 1) * 0.05f;
				p[1] = j * 0.05f;
				float value3 = ((noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22);
				p[0] = (i - 1) * 0.05f;
				p[1] = j * 0.05f;
				float value4 = ((noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22);
				p[0] = i * 0.05f;
				p[1] = (j + 1) * 0.05f;
				float value5 = ((noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22);
				p[0] = i * 0.05f;
				p[1] = (j - 1) * 0.05f;
				float value6 = ((noise.get(p, TCOD_NOISE_PERLIN) * 10) + 22);

				p[0] = i * 0.1f;
				p[1] = j * 0.1f;
				float value7 = ((noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15);
				p[0] = (i + 1) * 0.1f;
				p[1] = j * 0.1f;
				float value8 = ((noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15);
				p[0] = (i - 1) * 0.1f;
				p[1] = j * 0.1f;
				float value9 = ((noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15);
				p[0] = i * 0.1f;
				p[1] = (j + 1) * 0.1f;
				float value10 = ((noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15);
				p[0] = i * 0.1f;
				p[1] = (j - 1) * 0.1f;
				float value11 = ((noise.get(p, TCOD_NOISE_PERLIN) * 15) + 15);

				value = (int)(((1 + (1 - (biomeValue/4))) * (value2 + value3 + value4 + value5 + value6)) + value7 + value8 + value9 + value10 + value11) / 10;
				//COMPILER MAKE THIS VALUE NOT GO BELLOW ZERO
				value -= (std::max(riverValue * 2, 1.5f) - 1.5f) * 10;
				value = std::max(value, 2);
				//COMPILER JUST FUCKING DO IT

			for (int y = 0; y < value; y++) {
				if(biomeValue > 0.1){
					map->SetAt(i, j, y, TileManager::tile_dirt, TileManager::ground_empty);
					if (y < 10) {
						map->SetAt(i, j, y, TileManager::tile_stone, TileManager::ground_empty);
					}
				}
				else {
					map->SetAt(i, j, y, TileManager::tile_sand, TileManager::ground_empty);
					if (y < 5) {
						map->SetAt(i, j, y, TileManager::tile_stone, TileManager::ground_empty);
					}
				}
			}
			if (value > 13) {
				if (biomeValue > 0.1) {
					map->SetAt(i, j, value, TileManager::tile_grass, TileManager::ground_empty);
					map->SetAt(i, j, value + 1, TileManager::tile_grass, TileManager::ground_empty);
					map->SetGroundAt(i, j, value + 2, TileManager::ground_grass);
				}
				else {
					map->SetTileAt(i, j, value, TileManager::tile_sand);
					map->SetGroundAt(i, j, value + 1, TileManager::ground_sand);
				}
			}
			else{
					map->SetTileAt(i, j, value, TileManager::tile_dirt);
			}
			
			if (value > 15 && biomeValue > 0.1) {
				if (rand() % 100 < 1) {
					map->SetTileAt(i, j, value, TileManager::tile_wood);
					map->SetTileAt(i, j, value + 1, TileManager::tile_wood);
					map->SetTileAt(i, j, value + 2, TileManager::tile_wood);
					map->SetTileAt(i, j, value + 3, TileManager::tile_wood);

					map->SetTileAt(i - 1, j - 1, value + 4, TileManager::tile_leaves);
					map->SetTileAt(i - 1, j    , value + 4, TileManager::tile_leaves);
					map->SetTileAt(i - 1, j + 1, value + 4, TileManager::tile_leaves);

					map->SetTileAt(i    , j - 1, value + 4, TileManager::tile_leaves);
					map->SetTileAt(i    , j    , value + 4, TileManager::tile_leaves);
					map->SetTileAt(i    , j + 1, value + 4, TileManager::tile_leaves);

					map->SetTileAt(i + 1, j - 1, value + 4, TileManager::tile_leaves);
					map->SetTileAt(i + 1, j    , value + 4, TileManager::tile_leaves);
					map->SetTileAt(i + 1, j + 1, value + 4, TileManager::tile_leaves);

					map->SetTileAt(i - 1, j    , value + 5, TileManager::tile_leaves);

					map->SetTileAt(i    , j - 1, value + 5, TileManager::tile_leaves);
					map->SetTileAt(i    , j    , value + 5, TileManager::tile_leaves);
					map->SetTileAt(i    , j + 1, value + 5, TileManager::tile_leaves);

					map->SetTileAt(i + 1, j    , value + 5, TileManager::tile_leaves);

					map->SetTileAt(i    , j    , value + 6, TileManager::tile_leaves);
				}
			}
			else if (value > 15) {
				if (rand() % 20 < 1) {
					map->SetTileAt(i, j, value, TileManager::tile_cacti);
					map->SetTileAt(i, j, value + 1, TileManager::tile_cacti);
					map->SetTileAt(i, j, value + 2, TileManager::tile_cacti);
				}
			}
			
		}
	}

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			for (int y = 0; y < 30; y++) {
				if (map->GetTileAt(i, j, y) != nullptr && map->GetTileAt(i, j, y)->type == TileManager::tile_empty) {
					map->SetTileAt(i, j, y, TileManager::tile_water);
				}
			}
		}
	}

	r = tcodRand->getFloat(0, 1);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			float pBiome[2] = { (i * 0.22f) + r * 100, (j * 0.22f) + r * 100 };
			float val = noise.get(pBiome, TCOD_NOISE_PERLIN);

			if(val < 0.1){
				map->SetTileAt(i, j, 60 + ((val - 0.5) * 4), TileManager::tile_cloud);
				map->SetTileAt(i, j, 60 + ((val - 0.5) * 3), TileManager::tile_cloud);
				map->SetTileAt(i, j, 60 + ((val - 0.5) * 2), TileManager::tile_cloud);
				map->SetTileAt(i, j, 60 + ((val - 0.5) * 1), TileManager::tile_cloud);
			}
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
			map->SetTileAt(0, 0, 0, TileManager::tile_wall);
			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_dirt);
			}
			map->SetTileAt(i, j, value, TileManager::tile_grass);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
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

			for (int y = 0; y < value; y++) {
				map->SetTileAt(i, j, y, TileManager::tile_wall);
			}
			//SetTileAt(i, j, value, TileManager::floor);
		}
	}
}