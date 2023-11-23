#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

void generateUrlsArray(list * url_list, FILE * filePointer);
void generateTagsArray(tag_list * tag_list, FILE * filePointer);
void generatePathsArray(list * path_list, FILE * filePointer);
void generateAuth(char * username, char * password, FILE * filePointer);
void generatePreviousCode(FILE *filePointer);
void generateFollowingCode(FILE *filePointer);

void Generator(int result, symbol_table *symbolTable) {
    LogInfo("Generando archivo js. Compilación terminada con código: '%d'.", result);

    FILE *filePointer;

    filePointer = fopen("scrappy.js", "w");

    if (filePointer == NULL) {
        printf("Unable to create js file.\n");
        return;
    }

	generatePreviousCode(filePointer);
    // Generación de arrays
    generateUrlsArray(symbolTable->url_list, filePointer);
    generateTagsArray(symbolTable->tag_list, filePointer);
    generatePathsArray(symbolTable->path_list, filePointer);
	generateAuth(symbolTable->username, symbolTable->password, filePointer);

	generateFollowingCode(filePointer);

    fclose(filePointer);
}

void generateUrlsArray(list *url_list, FILE *filePointer) {
    fprintf(filePointer, "\nconst urls = [\n");

    node *current = url_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  '%s',\n", current->string);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

void generateTagsArray(tag_list *tag_list, FILE *filePointer) {
    fprintf(filePointer, "const tagIdPairs = [\n");

    tag_node *current = tag_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  { tag: '%s', id: '%s' },\n", current->name, current->id);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

void generatePathsArray(list *path_list, FILE *filePointer) {
    fprintf(filePointer, "const paths = [\n");

    node *current = path_list->head;

    while (current != NULL) {
        fprintf(filePointer, "  '%s',\n", current->string);
        current = current->next;
    }

    fprintf(filePointer, "];\n\n");
}

void generateAuth(char *username, char *password, FILE *filePointer) {
	if(username == NULL && password == NULL) {
		fprintf(filePointer, "const username = null;\n");
    	fprintf(filePointer, "const password = null;\n\n");
	} else {
		fprintf(filePointer, "const username = \'%s\';\n", username);
    fprintf(filePointer, "const password = \'%s\';\n\n", password);
	}
}

void generatePreviousCode(FILE *filePointer) {
    // Verificar si el puntero al archivo es válido
    if (filePointer == NULL) {
        return;
    }

    // Escribir el código JavaScript en el archivo
    fprintf(filePointer, "const fs = require('fs').promises;\n");
    fprintf(filePointer, "const puppeteer = require('puppeteer');\n");
    fprintf(filePointer, "const path = require('path');\n");
}

void generateFollowingCode(FILE *filePointer) {
    // Verificar si el puntero al archivo es válido
    if (filePointer == NULL) {
        return;
    }

    // Escribir el código JavaScript en el archivo
    fprintf(filePointer, "if (tagIdPairs.length === 0) {\n");
    fprintf(filePointer, "    console.error('Tag-id pairs array should not be empty.');\n");
    fprintf(filePointer, "    process.exit(1);\n");
    fprintf(filePointer, "}\n\n");

    fprintf(filePointer, "async function run() {\n");
    fprintf(filePointer, "    for (const url of urls) {\n");
    fprintf(filePointer, "        const folderName = encodeURIComponent(url);\n\n");

    fprintf(filePointer, "        for (const { tag, id } of tagIdPairs) {\n");
    fprintf(filePointer, "            try {\n");

    fprintf(filePointer, "                console.log(`Processing ${tag}#${id || 'no_id'}`);\n");
    fprintf(filePointer, "                const browser = await puppeteer.launch({ headless: 'new' });\n");
    fprintf(filePointer, "                const page = await browser.newPage();\n\n");

	fprintf(filePointer, "                if (username != null && password!= null) {\n");
	fprintf(filePointer, "                    await page.goto('https://campus.itba.edu.ar/');\n");
	fprintf(filePointer, "                    await page.type('#user_id', username);\n");
	fprintf(filePointer, "                    await page.type('#password', password);\n");
	fprintf(filePointer, "                    await page.click('#entry-login.button.expand.inverse.outline');\n");
	fprintf(filePointer, "                    await page.waitForNavigation();\n");
	fprintf(filePointer, "                }\n\n");

    fprintf(filePointer, "                await page.goto(url);\n\n");

    fprintf(filePointer, "                let data;\n\n");

    fprintf(filePointer, "                if (tag === 'img') {\n");
    fprintf(filePointer, "                    await page.waitForTimeout(1000);\n\n");

    fprintf(filePointer, "                    const issueSrcs = await page.evaluate(() => {\n");
    fprintf(filePointer, "                        const srcs = Array.from(document.querySelectorAll('img')).map(image => image.getAttribute('src'));\n");
    fprintf(filePointer, "                        return srcs;\n");
    fprintf(filePointer, "                    });\n\n");

    fprintf(filePointer, "                    const promises = paths.map(async (folder) => {\n");
    fprintf(filePointer, "                        const downloadPath = path.join(folder, folderName, `${tag}_${id || 'no_id'}`);\n\n");

    fprintf(filePointer, "                        await fs.mkdir(downloadPath, { recursive: true });\n");
    fprintf(filePointer, "                        await fs.writeFile(path.join(downloadPath, 'data.json'), JSON.stringify({ data: issueSrcs }, null, 2));\n");

    fprintf(filePointer, "                        console.log(`File saved for ${tag}#${id || 'no_id'} in ${downloadPath}`);\n");
    fprintf(filePointer, "                    });\n\n");

    fprintf(filePointer, "                    await Promise.all(promises);\n");
    fprintf(filePointer, "                } else {\n");
    fprintf(filePointer, "                    let selector = id ? `${tag}#${id}` : tag;\n\n");

    fprintf(filePointer, "                    if (tag === 'html') {\n");
    fprintf(filePointer, "                        data = await page.content();\n");
    fprintf(filePointer, "                    } else if (tag === 'head') {\n");
    fprintf(filePointer, "                        data = await page.evaluate(() => document.head.innerHTML);\n");
    fprintf(filePointer, "                    } else if (tag === 'title') {\n");
    fprintf(filePointer, "                        data = await page.evaluate(() => document.title);\n");
    fprintf(filePointer, "                    } else if (tag === 'body') {\n");
    fprintf(filePointer, "                        data = await page.evaluate(() => document.body.innerHTML);\n");
    fprintf(filePointer, "                    } else if (tag === 'div') {\n");
    fprintf(filePointer, "                        const divElement = await page.$(selector);\n");
    fprintf(filePointer, "                        data = divElement ? await divElement.evaluate(node => node.innerText) : null;\n");
    fprintf(filePointer, "                    } else {\n");
    fprintf(filePointer, "                        data = await page.evaluate((selector) => {\n");
    fprintf(filePointer, "                            const element = document.querySelector(selector);\n");
    fprintf(filePointer, "                            return element ? element.innerText : null;\n");
    fprintf(filePointer, "                        }, selector);\n");
    fprintf(filePointer, "                    }\n\n");

    fprintf(filePointer, "                    const promises = paths.map(async (folder) => {\n");
    fprintf(filePointer, "                        const downloadPath = path.join(folder, folderName, `${tag}_${id || 'no_id'}`);\n\n");

    fprintf(filePointer, "                        await fs.mkdir(downloadPath, { recursive: true });\n\n");

    fprintf(filePointer, "                        await fs.writeFile(path.join(downloadPath, 'data.json'), JSON.stringify({ data }, null, 2));\n\n");

    fprintf(filePointer, "                        console.log(`File saved for ${tag}#${id || 'no_id'} in ${downloadPath}`);\n");
    fprintf(filePointer, "                    });\n\n");

    fprintf(filePointer, "                    await Promise.all(promises);\n");
    fprintf(filePointer, "                }\n\n");

    fprintf(filePointer, "                await browser.close();\n");
    fprintf(filePointer, "            } catch (error) {\n");
    fprintf(filePointer, "                console.error(`Error processing ${tag}#${id || 'no_id'}: ${error}`);\n");
    fprintf(filePointer, "            }\n");
    fprintf(filePointer, "        }\n");
    fprintf(filePointer, "    }\n");
    fprintf(filePointer, "}\n\n");

    fprintf(filePointer, "run();\n");
}
