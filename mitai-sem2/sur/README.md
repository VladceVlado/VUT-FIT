# SUR 2019/2020

## Projekt

### Zadání

- Bodové ohodnocení: 25 bodů
- Úkolem je natrénovat detektor jedné osoby z obrázku obličeje a hlasové nahrávky.
- Trénovací vzory jsou k dispozici v archívu na [adrese](https://www.fit.vutbr.cz/study/courses/SUR/public/projekt_2019-2020/SUR_projekt2019-2020.tgz)

### Archív obsahuje:
- Trénovací vzory pro detekovanou osobu ve formátu PNG a WAV
    - `target_train/`
    - `target_dev/`

- Negativní příklady povolené pro trénování detektoru
    - `non_target_train/`
    - `non_target_dev/`

Rozdělení dat do adresářů `*_train/` a `*_dev/` je možné použít pro trénování a vyhodnocování úspěšnosti vyvíjeného detektoru. Toto rozdělení však není závazné (např. pomocí technik jako je jack-knifing lze efektivně trénovat i testovat na všech datech). Při pokusech o jiné rozdělení dat může být užitečné respektovat informace o tom, které trénovací vzory patří stejné osobě a zda-li byly pořízený v rámci jednoho nahrávacího sezení. Jméno každého souboru je rozděleno do poli pomocí podtržítek (např. `f401_01_f21_i0_0.png`), kde první pole (f401) je identifikátor osoby a druhé pole je číslo nahrávacího sezení (01).

Ostrá data, na kterých budou vaše systémy vyhodnoceny, budou k dispozici v pátek, 24. dubna ráno. Tato data budu obsahovat řádově stovky souboru ke zpracování. Vaším úkolem bude automaticky zpracovat tato data vašimi systémy (věříme Vám že nebudete podvádět a dívat se na obrázky čí poslouchat nahrávky) a uploadovat  soubory s výsledky do WISu. Soubor s výsledky bude ASCII se třemi poli na řádku oddělenými mezerou. Tato pole budou obsahovat popořadě následující údaje:

 - jméno segmentu (jméno souboru bez přípony .wav či .png)
 - číselné skóre, o kterém bude platit, že čím větší má hodnotu, tím si je  systém jistější, že se jedná o hledanou osobu
 - tvrdé rozhodnutí: číslo 1 pro hledanou osobu jinak 0. Toto rozhodnutí proveďte pro předpoklad, že apriorní pravděpodobnost výskytu hledané osoby v každém testovaném vzoru je 0,5

V jakém programovacím jazyce budete implementovat váš detektor či pomocí jakých nástrojů (spousta jich je volně k dispozici na Internetu) budete data zpracovávat záleží jen na Vás. Odevzdat můžete několik souborů s výsledky (např. pro systémy rozhodujícím se pouze na základě řečové nahrávky či pouze obrázku). Maximálně však námi bude zpracováno 5 takových souborů.

Soubory s výsledky můžete do soboty 25. dubna 23:59 uploadovat do WISu. Klíč se správnými odpověďmi bude zveřejněn 26. dubna. Na poslední přednášce 28. dubna 2020 bychom měli analyzovat Vaše výsledky a řešení.

Na tomto projektu budete pracovat ve skupinách (1-5 lidí), do kterých se můžete přihlásit ve WISu. Jména souborů s výsledky pro jednotlivé systémy volte tak, aby se podle nich dalo poznat o jaký systém se jedná (např. audio_GMM, image_linear). Každá skupina uploadne všechny soubory s výsledky zabalené do jednoho ZIP archívu se jménem `login1_login2_login3_login4_login5.zip` či `login1.zip`, podle toho, kolik Vás bude ve skupině. Kromě souborů s výsledky bude archív obsahovat také adresář `SRC/`, do kterého uložíte soubory se zdrojovými kódy implementovaných systémů. Dále bude archív obsahovat soubor `dokumentace.pdf`, který bude v českém, slovenském nebo anglickém jazyce popisovat Vaše řešení a umožní reprodukci Vaší práce. Důraz věnujte tomu, jak jste systémy během jejich vývoje vyhodnocovali, a které techniky či rozhodnutí se pozitivně projevily na úspěšnosti systému. Tento dokument bude také popisovat jak získat Vaše výsledky pomocí přiloženého kódu. Bude tedy uvedeno jak Vaše zdrojové kódy zkompilovat, jak vaše systémy spustit, kde hledat výsledné soubory, jaké případné externí nástroje je nutné instalovat a jak je přesně použít, atd. Očekávaný rozsah tohoto dokumentu jsou 3 strany A4. Do ZIP archívu prosím nepřikládejte evaluační data!

Inspiraci pro vaše systémy můžete hledat v archívu demonstračních příkladů pro předmět SUR: [link](https://www.fit.vutbr.cz/study/courses/SUR/public/prednasky/demos/).

Zvláště se podívejte na příklad detekce pohlaví z řeči: `demo_genderID.py`. Užitečné vám mohou být funkce pro načítaní PNG souborů (png2fea) a extrakci MFCC příznaků z WAV souborů (wav16khz2mfcc).

### Hodnocení:
- vše je odevzdáno a nějakým způsobem pracuje ... plný počet 25 bodů
    - čtou se soubory,
    - produkuje se skóre
    - klasifikátor nemusí (a nebude!) pracovat se 100% úspěšností, jsou to reálná data !
    - jsou odevzdány všechny požadované soubory v požadovaných formátech.
    - v dokumentaci vysvětlíte, co by se ještě dalo zlepšit.
- něco z výše uvedeného není splněno ? ... méně bodů.
