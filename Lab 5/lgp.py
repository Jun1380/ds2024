import glob

def find_files_with_extension(extension, search_path="."):
  """
  This function searches for files with a specific extension in the given path.
  """
  found_file_paths = []
  for file in glob.glob(f"{search_path}/*.{extension}"):
    found_file_paths.append(file)
  return found_file_paths

extension_to_find = input("Enter the file extension to find (e.g., txt, jpg): ")
search_path = input("Enter the search path (leave blank for current directory): ") or "."

found_file_paths = find_files_with_extension(extension_to_find, search_path)

if found_file_paths:
  print("File(s) found:")
  for file_path in found_file_paths:
    print(file_path)
else:
  print("No file(s) found with the specified extension.")

