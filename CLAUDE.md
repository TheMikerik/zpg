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
├── makefile        # sestavení a spuštění
├── src/            # zdrojové soubory .cpp
├── include/        # hlavičkové soubory .h
├── shaders/        # GLSL shadery (.glsl)
├── models/         # modely jako pole float v .h (+ login.obj/.mtl)
└── build/          # výstup kompilace (generované, negituj)
```

## Sestavení a spuštění

- `cd current && make` — zkompiluje projekt a rovnou ho spustí (`build/main`).
- `make clear` — smaže `build/main` a objektové soubory.
- Při přidání nového `.cpp` souboru je nutné ho doplnit do proměnné `SRCS` v `makefile`.

Kompilátor: `g++`, přepínače `-std=c++11 -Wall -g`. Knihovny z Homebrew: **GLFW**, **GLEW**, **GLM**, framework **OpenGL** (macOS).

## Architektura (jak to funguje)

1. **`main.cpp`** spustí `Application`.
2. **`Application`** vytvoří okno GLFW (800×600, OpenGL 3.3 Core), inicializuje GLEW a vytvoří všech 5 scén (`Scene1` až `Scene5`). Pak běží hlavní smyčka: vymaže obrazovku, zavolá `update(time)` a `render()` aktuální scény, prohodí buffery a zpracuje vstup.
3. **`Controller`** čte vstup od uživatele a podle něj přepíná aktuální scénu (`getCurrentSceneIndex()`).
4. **`Scene`** je jen seznam `drawables` (`std::vector<DrawableObject>`). Umí `update` a `render` pro všechny objekty.
5. **`DrawableObject`** spojuje tři věci:
   - `Model` — vrcholová data nahraná do VBO/VAO,
   - `ShaderProgram` — shadery (vertex + fragment),
   - `transformation` (statická transformace) a `dynamic` (funkce `time → Transformation`, která se volá každý snímek a dělá animaci).
6. **Transformace**:
   - `Transformation` drží matici 4×4 (GLM) a umí `translate`, `rotate` (ve stupních), `scale`.
   - `TransformGroup` skládá více transformací do jedné matice (`add` + `getMatrix`).
   - Vrcholový shader dostává matici jako uniform `modelMatrix` a počítá `gl_Position = modelMatrix * vec4(aPos, 1.0)`.
7. **Vrcholová data** jsou v `models/*.h` jako pole `float`. Každý vrchol má **9 hodnot**. Shader používá pozici `aPos` (location 0) a barvu `aColor` (location 1). Funkce `positionAndColor` v `Scene*.cpp` vybere z 9 hodnot prvních 6 (pozice + barva).

## Shadery

- `vertex_shader.glsl` — převádí pozici pomocí `modelMatrix` a předává barvu.
- `fragment_shader.glsl` — vypisuje barvu vrcholu.
- `fragment_shader_red/green/purple.glsl` — stejné, jen barva je přefiltrovaná na jednu složku.

Cesty k shaderům se berou relativně ke složce `current/`, proto se program musí spouštět přes `make` (nebo z `current/`).

## Konvence při psaní kódu

- **Jednoduchost nade vše.** Žádné složité šablony, zbytečné dědičnosti ani abstrakce navíc. Musí jít vysvětlit u zkoušky.
- **Styl podle okolního kódu:** 4 mezery odsazení, složené závorky na nových řádcích u funkcí, pojmenované konstanty místo „magických“ čísel (např. `SUN_RADIUS`).
- **Komentáře česky**, stručně a jen tam, kde pomáhají pochopit logiku (ne u každého řádku).
- Nové scény dělej stejným vzorem jako `Scene1`–`Scene5`: třída s `static Scene create()`, registrace v `Application::createScenes()`, přidání do `makefile`.
- Nepřidávej závislosti, které nejsou v projektu ani v zadání.

## Postup práce

1. Přečti zadání od uživatele a řekni, jak jsi ho pochopil (jedna až dvě věty).
2. Udělej jen změny, které zadání žádá, a jen ve složce `current/`.
3. Ověř, že se projekt zkompiluje (`cd current && make`). Pokud nejde spustit grafické okno, ověř aspoň kompilaci a přečti kód.
4. Na konci napiš, co jsi změnil a které volby jsi udělal sám, aby je uživatel mohl upravit.
5. Commit nebo push nedelej.
