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
│   └── scenes/      # Scene, SceneForest, ScenePlanets, SceneSpheres
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
2. **`Application`** vytvoří okno GLFW (800×600, OpenGL 3.3 Core), inicializuje GLEW a vytvoří 3 scény: `SceneForest`, `ScenePlanets` a `SceneSpheres`. Pak běží hlavní smyčka: vymaže obrazovku, zavolá `update(time)` a `render()` aktuální scény, prohodí buffery a zpracuje vstup.
3. **`Controller`** čte vstup od uživatele. Klávesy `1`–`3` přepínají scénu (`getCurrentSceneIndex()`), klávesa `Q` ukončí aplikaci.
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
9. **Světla** jsou ve třídě `Light` (`graphics/Light.h`). Zatím je to bodové světlo (pozice + barva), které posílá uniformy `lightPos` a `lightColor` přes `sendTo(shader)`.

## Shadery

- `vertex_shader.glsl` — převádí pozici pomocí matic a předává barvu vrcholu.
- `vertex_shader_light.glsl` — převádí pozici a předává světové souřadnice a normálu (pro osvětlení).
- `vertex_shader_tree.glsl`, `vertex_shader_clock.glsl` — specializované vrcholové shadery pro strom a hodiny.
- `fragment_shader.glsl` — vypisuje barvu vrcholu.
- `fragment_shader_color.glsl` — vypisuje uniform `objectColor`.
- `fragment_shader_light.glsl` — ambientní + difuzní osvětlení od `Light`, barva z `objectColor`.
- `fragment_shader_tree.glsl` — stínování podle normály, barvu listí z uniformu `leafColor`.

Cesty k shaderům se berou relativně ke složce `current/`, proto se program musí spouštět přes `make` (nebo z `current/`).

## Konvence při psaní kódu

- **Jednoduchost nade vše.** Žádné složité šablony ani zbytečné abstrakce. Dědičnost používáme jen tam, kde je zadání výslovně chce (například `Model` → `PositionColorModel`). Musí jít vysvětlit u zkoušky.
- **Styl:** 4 mezery odsazení, složené závorky u funkcí, tříd a řídicích příkazů na stejném řádku jako hlavička (`void f() {`). Soubor `.clang-format` v `current/` to nastavuje. Pojmenované konstanty místo „magických“ čísel (např. `SUN_RADIUS`).
- **Komentáře česky**, stručně a jen tam, kde pomáhají pochopit logiku (ne u každého řádku).
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
