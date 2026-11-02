#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

// List of target metadata fields to extract
const char *fields[] = {
    "pkgname", 
	"pkgver", 
	"pkgrel", 
	"pkgdesc",
    "url", 
	"arch", 
	"license", 
	"depends",
    "makedepends", 
	"optdepends", 
	"source", 
	"sha256sums"
};
#define NUM_FIELDS (sizeof(fields) / sizeof(fields[0]))

// Helper function to trim leading/trailing whitespace, quotes, and closing parentheses
void trim_string(char *str) {
    // Trim leading whitespace and quotes
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n' || *start == '\'' || *start == '"') {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    // Trim trailing whitespace, quotes, parentheses, and backslashes
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || 
                       str[len - 1] == '\n' || str[len - 1] == '\'' || str[len - 1] == '"' || 
                       str[len - 1] == ')' || str[len - 1] == '\\')) {
        str[len - 1] = '\0';
        len--;
    }
}

int main(int argc, char *argv[]) {
    // Default to 'PKGBUILD' if no filename argument is provided
    const char *filename = (argc > 1) ? argv[1] : "PKGBUILD";
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char current_key[64] = "";
    bool in_array = false;

    printf("Parsed PKGBUILD Metadata:\n");
    printf("-------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        // Strip newline characters at the end of the line
        line[strcspn(line, "\r\n")] = 0;

        // Skip leading whitespace for inspection
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;

        // Ignore comments or empty lines (unless actively parsing a multi-line array)
        if (*p == '#' || *p == '\0') {
            continue;
        }

        if (!in_array) {
            // Check if the line begins with any of our target fields (e.g., "pkgname=")
            for (size_t i = 0; i < NUM_FIELDS; i++) {
                char prefix[128];
                snprintf(prefix, sizeof(prefix), "%s=", fields[i]);
                
                if (strncmp(p, prefix, strlen(prefix)) == 0) {
                    strcpy(current_key, fields[i]);
                    char *val_start = p + strlen(prefix);
                    
                    // Skip whitespace after '='
                    while (*val_start == ' ' || *val_start == '\t') val_start++;

                    // Check if this starts a multi-line or single-line array `(...)`
                    if (*val_start == '(') {
                        in_array = true;
                        val_start++; // Skip opening parenthesis
                        printf("\n[%s] (Array):\n", current_key);
                    } else {
                        // Regular string value (e.g., pkgver=2.70.0)
                        trim_string(val_start);
                        printf("\n[%s]: %s\n", current_key, val_start);
                        current_key[0] = '\0';
                    }

                    // Check if the array closes on the exact same line
                    if (in_array && strchr(val_start, ')')) {
                        in_array = false;
                    }
                    break;
                }
            }
        } else {
            // We are currently inside a multi-line array block
            char *item = p;
            
            // Check if the closing parenthesis `)` appears on this line
            char *end_paren = strchr(item, ')');
            if (end_paren) {
                *end_paren = '\0';
                in_array = false;
            }

            // Clean up and print the array element
            trim_string(item);
            if (strlen(item) > 0) {
                printf("  -> %s\n", item);
            }
        }
    }

    fclose(file);
    return 0;
}
