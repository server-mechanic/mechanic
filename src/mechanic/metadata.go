/*
 * Copyright (c) 2016 Cornelius Buschka.
 * Server Mechanic (http://server-mechanic.org)
 *
 * This file is part of Server Mechanic.
 *
 * Server Mechanic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
package mechanic

import (
	"os"
	"syscall"
)

type Version struct {
	version string
}

func RunFollowUpCommand(followUpCommand []string) error {

	if len(followUpCommand) > 0 {
		env := os.Environ()
		err := syscall.Exec(followUpCommand[0], followUpCommand[0:], env)
		return err
	}

	return nil
}
