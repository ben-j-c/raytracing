
clear
clc

files = dir;
for i = 1:numel(files)
	splitfile = split(files(i).name, '.');
	extension = splitfile{end};
	if(strcmp(extension, 'ppm'))
		importfile(files(i).name);
		figure;
		imshow(eval(splitfile{1}));
		title(splitfile{1});
	end
end

clear i
clear files
clear extension
clear splitfile