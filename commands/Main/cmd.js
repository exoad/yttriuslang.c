const fs = require('fs');
const {
  promisify
} = require('util');
const readdir = promisify(fs.readdir);
const Discord = require("discord.js");
const bot = new Discord.Client();
const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const resource = require('../../configs/resource.json');
module.exports = {
    config: {
        name: `cmd`,
        description: '???',
        category: '???',
        aliases: [``]
    },
    
    run: async (bot, message, args) => {

        const command = message.content.split(' '); 
        if(command[1] == undefined){
            const embed = new Discord.MessageEmbed()
                .setColor("RANDOM")
                .setTitle("Oops, looks like you forgot some parameters")
                .setThumbnail(resource.aw_snap)
                .addField("Usage:", "``"+config.prefix+"command {command_name}``")
                .addField("Description:", "Get indepth information on commands. Use ``"+config.prefix+"help`` for a list of commands");
            message.channel.send(embed);    
        }

        if(command){
          if(command[1] != undefined){ 
              fs.readFile('./configs/commands.json', (err, dataJson) => {  
                  if (err) throw err;
                  let helpMe = JSON.parse(dataJson);
                  let commandName = command[1];
      
                  try{
                      const embed = new MessageEmbed()
                          .setColor("RANDOM")
                          .addField(`Command Name: `, `\`${helpMe[commandName].name}\``)
                          .addField(`Category`, `\`${helpMe[commandName].group}\``)
                          .addField("Description", `\`${helpMe[commandName].description}\``)
                          .addField("Usage", `\`${config.prefix}${helpMe[commandName].usage}\``)
                          .addField("Aliases", `\`${helpMe[commandName].alias}\``)
                      message.channel.send(embed);    
                  }
                  catch(err){
                    const embed = new MessageEmbed()
                    .setTitle("Whoops, looks like the requested command doesn't exist (yet)!")
                    .setThumbnail(resource.aw_snap)
                    .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands!")
                    .setFooter("Still facing issues? Join the support server!")
                      message.channel.send({ embed }).then(m => {
                          m.delete({ timeout: 5000 })
                      });
                  }
              });
            }
        }
    }
}