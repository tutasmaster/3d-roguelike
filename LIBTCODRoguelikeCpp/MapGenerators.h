#pragma once

#include "Map.h"

void GeneratePerlinNoiseMap(Map * map) {
	TCODRandom* tcodRand = new TCODRandom();
	TCODNoise noise(2, tcodRand);

	float r = tcodRand->getFloat(0,1);

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			
			float pBiome[2] = { (i * 0.01) + r * 100, (j * 0.01) + r * 100 };
			float biomeValue = noise.get(pBiome, TCOD_NOISE_PERLIN);

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

			for (int h = 0; h < value; h++) {
				if(biomeValue > 0.1){
					map->SetTileAt(i, j, h, TileManager::dirt);
					if (h < 10) {
						map->SetTileAt(i, j, h, TileManager::stone);
					}
				}
				else {
					map->SetTileAt(i, j, h, TileManager::sand);
					if (h < 5) {
						map->SetTileAt(i, j, h, TileManager::stone);
					}
				}
			}
			if (value > 13) {
				if (biomeValue > 0.1) {
					map->SetTileAt(i, j, value, TileManager::grass);
				}
				else {
					map->SetTileAt(i, j, value, TileManager::sand);
				}
			}
			else{
					map->SetTileAt(i, j, value, TileManager::dirt);
			}
			
			if (value > 15 && biomeValue > 0.1) {
				if (rand() % 100 < 1) {
					map->SetTileAt(i, j, value, TileManager::wood);
					map->SetTileAt(i, j, value + 1, TileManager::wood);
					map->SetTileAt(i, j, value + 2, TileManager::wood);
					map->SetTileAt(i, j, value + 3, TileManager::wood);

					map->SetTileAt(i - 1, j - 1, value + 4, TileManager::leaves);
					map->SetTileAt(i - 1, j    , value + 4, TileManager::leaves);
					map->SetTileAt(i - 1, j + 1, value + 4, TileManager::leaves);

					map->SetTileAt(i    , j - 1, value + 4, TileManager::leaves);
					map->SetTileAt(i    , j    , value + 4, TileManager::leaves);
					map->SetTileAt(i    , j + 1, value + 4, TileManager::leaves);

					map->SetTileAt(i + 1, j - 1, value + 4, TileManager::leaves);
					map->SetTileAt(i + 1, j    , value + 4, TileManager::leaves);
					map->SetTileAt(i + 1, j + 1, value + 4, TileManager::leaves);

					map->SetTileAt(i - 1, j    , value + 5, TileManager::leaves);

					map->SetTileAt(i    , j - 1, value + 5, TileManager::leaves);
					map->SetTileAt(i    , j    , value + 5, TileManager::leaves);
					map->SetTileAt(i    , j + 1, value + 5, TileManager::leaves);

					map->SetTileAt(i + 1, j    , value + 5, TileManager::leaves);

					map->SetTileAt(i    , j    , value + 6, TileManager::leaves);
				}
			}
			else if (value > 15) {
				if (rand() % 20 < 1) {
					map->SetTileAt(i, j, value, TileManager::cacti);
					map->SetTileAt(i, j, value + 1, TileManager::cacti);
					map->SetTileAt(i, j, value + 2, TileManager::cacti);
				}
			}
			
		}
	}

	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			for (int h = 0; h < 20; h++) {
				if (map->GetTileAt(i, j, h) != nullptr && map->GetTileAt(i, j, h)->type == TileManager::empty) {
					map->SetTileAt(i, j, h, TileManager::water);
				}
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
			map->SetTileAt(0, 0, 0, TileManager::wall);
			for (int h = 0; h < value; h++) {
				map->SetTileAt(i, j, h, TileManager::dirt);
			}
			map->SetTileAt(i, j, value, TileManager::grass);
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