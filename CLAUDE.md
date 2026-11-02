# CLAUDE.md

Popis projektu pro Claude Code. Piš vždy česky.

## O projektu

Školní projekt do předmětu **Základy počítačové grafiky (ZPG)** na univerzitě.
Jde o jednoduchou 3D aplikaci v **OpenGL 3.3 Core Profile** v jazyce **C++11**.
Projekt se hodnotí i ústně, takže kód musí být jednoduchý a dát se vysvětlit.

Konkrétní zadání přijde vždy ve zprávě od uživatele. Dělej **jen to, co je v zadání**. Nic navíc, žádné „vylepšení“ mimo zadání.

## Pravidla pro práci se složkami

- **Veškerý kód a změny dělej jen ve složce `current/`.**
- **`previous/`** obsahuje úkoly z minulých cvičení. **Nikdy ji neupravuj.** Ignoruj ji.
- **`reference/`** obsahuje referenční projekt z minulého roku. Je jen na čtení. Můžeš se z ní inspirovat a implementovat podobně, ale nikdy do ní nezapisuj.
- Ostatní soubory mimo `current/` neměň (například `.gitignore`, `.vscode/`), pokud o to uživatel výslovně nepožádá.

## Struktura `current/`

```
current/
├── makefile         # sestavení a spuštění
├── .clang-format    # styl formátování (složené závorky u funkcí na stejném řádku)
├── src/
│   ├── common/      # Application, Controller, main.cpp
│   ├── graphics/    # Camera, DrawableObject, Light, Model, PositionColorModel, PositionNormalColorModel, ShaderProgram, Transformation, TransformGroup, OBJLoader
│   └── scenes/      # Scene, SceneForest, ScenePlanets, SceneSpheres, SceneNormals, SceneSpecular, SceneNightForest
├── include/         # hlavičky .h se stejnou podsložkou jako ve src/
├── shaders/         # GLSL shadery (.glsl)
├── models/          # modely jako pole float v .h (+ login.obj/.mtl)
└── build/           # výstup kompilace (generované, negituj)
```

## Sestavení a spuštění

- `cd current && make` — zkompiluje projekt a rovnou ho spustí (`build/main`).
- `make build/main` — jen zkompiluje a slinkuje, nespustí aplikaci.
- `make clear` — smaže `build/main` a objektové soubory.
- Při přidání nového `.cpp` souboru je nutné ho doplnit do proměnné `SRCS` v `makefile`.
- Pokud přidáš nový adresář s hlavičkami, doplň ho do `INCLUDES` v `makefile` (`-Iinclude/...`).

Kompilátor: `g++`, přepínače `-std=c++11 -Wall -g`. Knihovny z Homebrew: **GLFW**, **GLEW**, **GLM**, framework **OpenGL** (macOS).

## Architektura (jak to funguje)

1. **`main.cpp`** spustí `Application`.
2. **`Application`** vytvoří okno GLFW (800×600, OpenGL 3.3 Core), inicializuje GLEW a vytvoří 6 scén: `SceneForest`, `ScenePlanets`, `SceneSpheres`, `SceneNormals`, `SceneSpecular` a `SceneNightForest`. Pak běží hlavní smyčka: nastaví barvu pozadí (`Scene::clearColor`), vymaže obrazovku, zavolá `update(time)` a `render()` aktuální scény, prohodí buffery a zpracuje vstup.
3. **`Controller`** čte vstup od uživatele. Klávesy `1`–`6` přepínají scénu (`getCurrentSceneIndex()`), klávesa `Q` ukončí aplikaci, klávesa `R` přepíná chybný režim změny velikosti okna (`isResizeBugMode()`: zamrzne viewport a poměr stran).
4. **`Scene`** je jen seznam `drawables` (`std::vector<DrawableObject>`). Umí `update` a `render` pro všechny objekty.
5. **`DrawableObject`** spojuje čtyři věci:
   - `Model` — vrcholová data nahraná do VBO/VAO,
   - `ShaderProgram` — shadery (vertex + fragment),
   - `transformation` (statická transformace) a `dynamic` (funkce `time → Transformation`, která se volá každý snímek a dělá animaci),
   - `updateUniforms` — funkce, která před vykreslením nastaví uniformy shaderu (barva, světlo, úhly hodin).
6. **Model** je základní třída pro vrcholová data. Rozložení vrcholu zadávají odvozené třídy:
   - `PositionColorModel` — vrchol má **6 hodnot** (pozice, barva),
   - `PositionNormalColorModel` — vrchol má **9 hodnot** (pozice, normála, barva).
7. **Transformace**:
   - `Transformation` drží matici 4×4 (GLM) a umí `translate`, `rotate` (ve stupních), `scale`.
   - `TransformGroup` skládá více transformací do jedné matice (`add` + `getMatrix`).
   - Vrcholový shader dostává matici jako uniform `modelMatrix` a počítá `gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0)`.
8. **Vrcholová data** jsou v `models/*.h` jako pole `float`. Shader používá pozici `aPos` (location 0) a další atributy podle modelu.
9. **Světla** jsou ve třídě `Light` (`graphics/Light.h`). Bodové světlo (pozice, koeficienty utlumu). `sendTo(shader)` nastaví uniformy `lightCount = 1`, `lightPos`, `attenuation`. Barva světla (bílá) a ambientní síla (0.3) jsou pevné konstanty v `fragment_shader_light.glsl`. Shader počítá osvětlení jen pokud `lightCount >= 1`, jinak vypíše jen barvu. Výčet `ShadingModel` (`LAMBERT`, `PHONG`, `BLINN`) je v `Light.h`. Ve scéně `ScenePlanets` je to Slunce v počátku.

## Shadery

Shadery jsou sjednocené, rozlišení chování řídí uniformy. Většina scén používá jen tyto:

- `vertex_shader_light.glsl` — převádí pozici, počítá světové souřadnice `FragPos` a normálu (`Normal`) a předává barvu `vertexColor`. Uniformy:
  - `normalFromAttr` — `1` = normála je v datech (location 1, např. UFO, koule s daty), `0` = normála je `normalize(aPos)` (koule se středem v počátku).
  - `wrongNormalMatrix` — `1` = ukázka chyby: normála se převádí jen `mat3(modelMatrix)`. Výchozí `0` = správně přes `mat3(transpose(inverse(modelMatrix)))`.
- `fragment_shader_light.glsl` — jediný fragment shader pro všechny objekty:
  - barva: `objectColor`, nebo barva z vrcholů když `useVertexColor = 1`,
  - bez světla (`lightCount < 1`) se vypíše barva (původní `fragment_shader_color` / `fragment_shader`),
  - se světlem: ambientní složka + difuzní složka s útlumem `att = 1 / (a + b·d + c·d²)` (uniform `attenuation`), zrcadlová složka podle `shadingModel` (`0` Lambert, `1` Phong, `2` Blinn), lesk `shininess` je konstanta 32, uniform `shininess` ji přepíše jen tam, kde je nastaven (`SceneSpecular`, hodnota 1),
  - `specularOnBackSide = 1` = ukázka chyby (zrcadlová složka i na odvrácené straně); výchozí `0` = ořez tam, kde `diff == 0`.
- Scéna `SceneSpheres`: 4 koule se stejnými shadery, jen jiný `shadingModel`: červená bez světla (jen `objectColor`), zelená `LAMBERT`, modrá `PHONG`, žlutá `BLINN`. Světlo je ve středu.
- `vertex_shader_tree.glsl`, `fragment_shader_tree.glsl` — strom (listí podle `gl_VertexID`, stínování podle normály, barva listí z uniformu `leafColor`). Specifické, zachovány.
- `vertex_shader_clock.glsl` — ručičky hodin (rotace podle barvy vrcholu). Specifické, zachováno. Fragment je společný `fragment_shader_light.glsl` s `useVertexColor = 1` a bez světla.
- Scéna `ScenePlanets`: Slunce (`vertex_shader_light.glsl` + `fragment_shader_light.glsl`, bez světla, barva z vrcholů). Planety a Měsíc používají stejné shadery s `shadingModel = PHONG` a barvou z vrcholů. Slunce je zdroj světla ve středu scény (`Light` v pozici `(0, 0, 0)`). Scéna také obsahuje UFO načtené z `models/ufo.obj` přes `OBJLoader::loadOBJ` (`normalFromAttr = 1`, `PHONG`, jednolitá barva z `ufo.mtl`).

Cesty k shaderům se berou relativně ke složce `current/`, proto se program musí spouštět přes `make` (nebo z `current/`).

## Konvence při psaní kódu

- **Jednoduchost nade vše.** Žádné složité šablony ani zbytečné abstrakce. Dědičnost používáme jen tam, kde je zadání výslovně chce (například `Model` → `PositionColorModel`). Musí jít vysvětlit u zkoušky.
- **Styl:** 4 mezery odsazení, složené závorky u funkcí, tříd a řídicích příkazů na stejném řádku jako hlavička (`void f() {`). Soubor `.clang-format` v `current/` to nastavuje. Pojmenované konstanty místo „magických“ čísel (např. `SUN_RADIUS`).
- **Komentáře** nepiš, pokud logika nebo účel kódu není nezbytně nepochopitelný. Pak napiš jen krátký komentář česky, jednou větou.
- Scéna `SceneNormals`: dvě koule (`models/sphere.h`, `PositionNormalModel`) s nerovnoměrným škálováním `(0.3, 0.1, 0.15)`, `fragment_shader_light.glsl` s `PHONG`, světlo `(0, 1, 1)`. Vlevo chybný převod normál (`wrongNormalMatrix = 1`), vpravo správný. Ukazuje, že `mat3(modelMatrix) * normala` je správně jen pro shodné (rovnoměrné) škálování.
- Scéna `SceneSpecular`: dvě koule s `PositionNormalModel`, mocnina `shininess = 1`, světlo `(0, 0, -2)` za koulemi (přivrácená strana je ve stínu). Vlevo zrcadlová složka i na odvrácené straně (`specularOnBackSide = 1`, chyba), vpravo ořezaná tam, kde `diff == 0`.
- Změna velikosti okna: `Application::run()` každý snímek volá `glViewport` a `camera.setAspectRatio` (správně). Klávesa `R` (`Controller::isResizeBugMode()`) tuto aktualizaci vypne, takže se viewport a poměr stran zamrznou a obraz se při změně velikosti zkresluje.
- Scéna `SceneNightForest` (noční les, klávesa `6`): tmavé pozadí (`clearColor`), zem a mřížka stromů (`PositionNormalModel`, `vertex_shader_light.glsl` + `fragment_shader_light.glsl`, Lambert). Jediné světlo je světluška (`Light` na pevné pozici s útlumem `I = 1 / (1 + 0.7·d + 1.8·d²)`; barva světla je stejně bílá jako ve zbytku projektu). Světluška sama je nesvítící koule (bez `lightCount`, jen `objectColor`). Zem je škálovaná rovnoměrně.
- Nové scény dělej stejným vzorem jako `SceneForest`, `ScenePlanets` a `SceneSpheres`: třída s `static Scene create()`, registrace v `Application::createScenes()`, přidání do `makefile`, přepínání klávesou v `Controller` (`SCENE_COUNT`).
- Nepřidávej závislosti, které nejsou v projektu ani v zadání.

## Udržování tohoto souboru

- Pokud dojde k **zásadní změně** — změně logiky, fungování nebo cíle aplikace (například nová scéna, nový typ světla, změna ovládání nebo struktury tříd) — Claude tuto změnu sám zapíše do tohoto souboru do odpovídající sekce.
- Běžné změny kódu (opravy, přejmenování, formátování, drobné refaktory) sem nezapisuj, pokud nemění popis výše.

## Postup práce

1. Přečti zadání od uživatele a řekni, jak jsi ho pochopil (jedna až dvě věty).
2. Udělej jen změny, které zadání žádá, a jen ve složce `current/`.
3. Ověř, že se projekt zkompiluje (`cd current && make build/main`). Pokud nejde spustit grafické okno, ověř aspoň kompilaci a přečti kód.
4. Na konci napiš, co jsi změnil a které volby jsi udělal sám, aby je uživatel mohl upravit.
5. Commit nebo push nedelej.
